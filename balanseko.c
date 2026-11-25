#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "balanseko.h"

/* ---------- SIMPLE INPUT HELPERS ---------- */

/* Reads one line safely. Quits program if input ends (EOF). */
static void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) exit(0);
    if (strchr(buf, '\n') == NULL) {       /* line was too long: throw away the rest */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/* Keeps asking until the user types a whole number between min and max */
int read_int(const char *prompt, int min, int max) {
    char buf[100], extra;
    int n;
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if (sscanf(buf, "%d %c", &n, &extra) == 1 && n >= min && n <= max) return n;
        printf("Invalid input. Enter a number from %d to %d.\n", min, max);
    }
}

/* Keeps asking until the user types a positive amount */
static long long read_amount(const char *prompt) {
    char buf[100], *end;
    long long n;
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
              n = strtoll(buf, &end, 10);   /* end points to the first character that is not a digit */
      if (end != buf && (*end == '\n' || *end == '\0') && n > 0 && n <= MAX_AMOUNT) return n;
        printf("Invalid amount. Enter a whole number from 1 to %lld.\n", MAX_AMOUNT);
    }
}

/* Keeps asking until the user types a non-empty text that fits */
static void read_text(const char *prompt, char *out, int size) {
    char buf[256];
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] != '\0' && (int)strlen(buf) < size) {
            strcpy(out, buf);
            return;
        }
        printf("Invalid description. Must not be empty and max %d characters.\n", size - 1);
    }
}

/* Keeps asking until the user types y or n. Returns 1 for yes, 0 for no. */
int read_yes_no(const char *prompt) {
    char buf[100];
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if ((buf[0] == 'y' || buf[0] == 'Y') && (buf[1] == '\n' || buf[1] == '\0')) return 1;
        if ((buf[0] == 'n' || buf[0] == 'N') && (buf[1] == '\n' || buf[1] == '\0')) return 0;
        printf("Invalid input. Type y or n.\n");
    }
}

/* ---------- ACCOUNT HELPERS ---------- */

/* Returns the chart position of a code, or -1 if the code does not exist */
static int find_account(BalanseKoSystem *sys, int code) {
    for (int i = 0; i < 9; i++) {
        if (sys->chart[i].code == code) return i;
    }
    return -1;
}

/* Turns a code into its account name (used when printing) */
static const char *get_account_name(BalanseKoSystem *sys, int code) {
    int i = find_account(sys, code);
    if (i < 0) return "Unknown Account";
    return sys->chart[i].name;
}

/* Balance of one account (by its position in the chart), on its normal side */
static long long account_balance(BalanseKoSystem *sys, int i) {
    int code = sys->chart[i].code;
    long long debit = 0;
    long long credit = 0;

    for (int j = 9; j < sys->entry_count; j++) {
        if (sys->entries[j].debit_code == code) debit += sys->entries[j].debit_amount;
        if (sys->entries[j].credit_code == code) debit += sys->entries[j].credit_amount;
    }

    if (sys->chart[i].is_debit_normal == 1) return debit - credit;
    return credit - debit;
}

/* Keeps asking until the user types a code that exists in the chart */
static int read_account(BalanseKoSystem *sys, const char *prompt) {
    int code;
    while (1) {
        code = read_int(prompt, 1, 9999);
        if (find_account(sys, code) >= 0) return code;
        printf("Account code not found. Choose from the chart above.\n");
    }
}

static int days_in_month(int m, int y) {
    int d[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)) return 29;
    return d[m - 1];
}

/* Reads the date as one line: MM/DD/YYYY. Keeps asking until it is a real date */   
static void read_date(JournalEntry *e) {
    char buf[100], extra;
    while (1) {
        printf("Date (MM/DD/YYYY): ");
        read_line(buf, sizeof(buf));
        if (sscanf(buf, "%d/%d/%d %c", &e->month, &e->day, &e->year, &extra) == 3 
            && e->month >= 1 && e->month <= 12
            && e->year >= 1900 && e->year <= 2100
            && e->day >= 1 && e->day <= days_in_month(e->month, e->year)) {
            return;
        }
        printf("Invalid date. Use MM/DD/YYYY (example: 03/15/2026).\n");
    }
}

/* ---------- SYSTEM ---------- */

void init_system(BalanseKoSystem *sys) {
    sys->entry_count = 0;
    // 1 = Normal Debit (Asset/Expense), 0 = Normal Credit (Liab/Eq/Rev)*/
    Account fixed_chart[9] = {
        {101, "Cash", 1}, {102, "Accounts Receivable", 1}, {103, "Equipment", 1},
        {201, "Accounts Payable", 0}, {301, "Owner's Capital", 0},
        {401, "Service Revenue", 0}, {501, "Rent Expense", 1},
        {502, "Salaries Expense", 1}, {503, "Utilities Expense", 1}
    };

    for (int i = 0; i < 9; i++) {
        sys->chart[i] = fixed_chart[i];
    }
}

void show_chart(BalanseKoSystem *sys) {
    printf("\n--- CHART OF ACCOUNTS ---\n");
    printf("Code | Account Name\n");
    printf("-------------------------\n");
    for (int i = 0; i < 9; i++) {
        printf("%d  | %s\n", sys->chart[i].code, sys->chart[i].name);
    }
}

/* ---------- ENTRY INPUT (used by Add and Edit) ---------- */

/* Asks for every field. Returns 1 if the entry is good, 0 if cancelled. */
static int input_entry(BalanseKoSystem *sys, JournalEntry *e) {
    int ok;

    show_chart(sys);
    printf("\n"); /* blank line between the chart and the date */

    read_date(e);

    read_text("Description: ", e->description, sizeof(e->description));

    e->debit_code = read_account(sys, "Debit Account Code: ");
    e->debit_amount = read_amount("Debit Amount: ");

    do {
        e->credit_code = read_account(sys, "Credit Account Code: ");
        ok = (e->credit_code != e->debit_code);
        if (!ok) printf("Debit and Credit accounts must be different.\n");
    } while (!ok);
    e->credit_amount = read_amount("Credit Amount: ");

    if (e->debit_amount != e->credit_amount) {
        printf("Error: Debits (%.lld) must equal Credits (%.lld). Entry cancelled.\n",
               e->debit_amount, e->credit_amount);
        return 0;
    }
    return 1;
}

void add_entry(BalanseKoSystem *sys) {
    if (sys->entry_count >= MAX_ENTRIES) {
        printf("Journal is full!\n");
        return;
    }

    JournalEntry e;
    printf("\n--- ADD JOURNAL ENTRY ---\n");
    if (input_entry(sys, &e)) {
        sys->entries[sys->entry_count] = e;
        sys->entry_count++;
        printf("Entry saved successfully!\n");
    }
}

/* ---------- VIEW ---------- */

void view_journal(BalanseKoSystem *sys) {
    printf("\n--- GENERAL JOURNAL ---\n");
    if (sys->entry_count == 0) {
        printf("No entries yet.\n");
        return;
    }
    for (int i = 0; i < sys->entry_count; i++) {
        JournalEntry e = sys->entries[i];
        printf("\n#%d | Date: %02d/%02d/%04d | Desc: %s\n", i + 1, e.month, e.day, e.year, e.description);
        printf("  Debit:  %s - %.lld\n", get_account_name(sys, e.debit_code), e.debit_amount);
        printf("  Credit: %s - %.lld\n", get_account_name(sys, e.credit_code), e.credit_amount);
    }
}

/* ---------- EDIT / DELETE MENU ---------- */

/* Does ONE edit or ONE delete, then returns (main.c asks if the user wants to stay) */
void manage_entries(BalanseKoSystem *sys) {
    int choice, num;

    printf("\n--- EDIT / DELETE JOURNAL ENTRY ---\n");
    printf(" 1. Edit an entry\n");
    printf(" 2. Delete an entry\n");
    printf(" 0. Cancel\n");
    choice = read_int("Choice: ", 0, 2);

    if (choice == 0) return;

    if (sys->entry_count == 0) {
        printf("No entries yet.\n");
        return;
    }

    view_journal(sys);
    num = read_int("\nEntry number: ", 1, sys->entry_count);

    if (choice == 1) {
        JournalEntry temp;
        printf("\nEnter the new details for entry #%d:\n", num);
        if (input_entry(sys, &temp)) {
            sys->entries[num - 1] = temp;
            printf("Entry #%d updated successfully!\n", num);
        } else {
            printf("No changes made.\n");
        }
    } else {
        if (read_yes_no("Are you sure you want to delete this entry? (y/n): ")) {
            for (int i = num - 1; i < sys->entry_count - 1; i++) {
                sys->entries[i] = sys->entries[i + 1];
            }
            sys->entry_count--;
            printf("Entry deleted.\n");
        } else {
            printf("Delete cancelled.\n");
        }
    } 
}

/* ---------- TRIAL BALANCE ---------- */

void view_trial_balance(BalanseKoSystem *sys) {
    long long total_debits = 0;
    long long total_credits = 0;

    printf("\n--- TRIAL BALANCE ---\n");
    printf("%-20s | %-10s | %-10s\n", "Account", "Debit", "Credit");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < 9; i++) {
        long long balance = account_balance(sys, i);

        if (balance != 0) {
            if (sys->chart[i].is_debit_normal == 1) {
                printf("%-20s | %-18lld | %-18s\n", sys->chart[i].name, balance, "");
                total_debits += balance;
            } else {
                printf("%-20s | %-18s | %-18lld\n", sys->chart[i].name, "", balance);
                total_credits += balance;
            }
        }
    }

    printf("--------------------------------------------------------------\n");
    printf("%-20s | %-18lld | %-18lld\n", "TOTAL", total_debits, total_credits);
}

/* ---------- FINANCIAL STATEMENTS ---------- */
/* Account types by first digit of the code:
   1 = Assets, 2 = Liabilities, 3 = Equity, 4 = Revenue, 5 = Expenses */

/* Adds up the balances of one account type (prints nothing) */
static long long section_total(BalanseKoSystem *sys, int type) {
    long long total = 0;

    for (int i = 0; i < 9; i++) {
        if (sys->chart[i].code / 100 == type) {
            total += account_balance(sys, i);
        }
    }
    return total;
}

/* Prints each account of one account type, one per line */
static void print_section(BalanseKoSystem *sys, int type) {
    for (int i = 0; i < 9; i++) {
        if (sys->chart[i].code / 100 == type) {
            printf("  %-25s %20lld\n", sys->chart[i].name, account_balance(sys, i));
        }
    }
}

void view_income_statement(BalanseKoSystem *sys) {
    long long total_revenue  = section_total(sys, 4);
    long long total_expenses = section_total(sys, 5);
    long long net_income     = total_revenue - total_expenses;

    printf("\n--- INCOME STATEMENT ---\n");

    printf("Revenue\n");
    print_section(sys, 4);
    printf("%-27s %20lld\n", "Total Revenue", total_revenue);

    printf("\nExpenses\n");
    print_section(sys, 5);
    printf("%-27s %20lld\n", "Total Expenses", total_expenses);

    printf("------------------------------------------------\n");
    if (net_income >= 0) {
        printf("%-27s %20lld\n", "NET INCOME", net_income);
    } else {
        printf("%-27s %20lld\n", "NET LOSS", -net_income);
    }
}

void view_balance_sheet(BalanseKoSystem *sys) {
    long long total_assets      = section_total(sys, 1);
    long long total_liabilities = section_total(sys, 2);
    long long net_income        = section_total(sys, 4) - section_total(sys, 5);
    /* Net income is added to equity because we do not do closing entries */
    long long total_equity      = section_total(sys, 3) + net_income;

    printf("\n--- BALANCE SHEET ---\n");

    printf("Assets\n");
    print_section(sys, 1);
    printf("%-27s %20lld\n", "TOTAL ASSETS", total_assets);

    printf("\nLiabilities\n");
    print_section(sys, 2);
    printf("%-27s %20lld\n", "Total Liabilities", total_liabilities);

    printf("\nOwner's Equity\n");
    print_section(sys, 3);
    printf("  %-25s %20lld\n", "Net Income (Loss)", net_income);
    printf("%-27s %20lld\n", "Total Owner's Equity", total_equity);

    printf("------------------------------------------------\n");
    printf("%-27s %20lld\n", "TOTAL LIABILITIES + EQUITY", total_liabilities + total_equity);
}