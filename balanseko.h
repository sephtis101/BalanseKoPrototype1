#ifndef BALANSEKO_H
#define BALANSEKO_H

#define MAX_ENTRIES 200
#define MAX_AMOUNT 1000000000000000LL

/* Fixed Account Structure*/
typedef struct {
    int code;
    char name[50];
    int is_debit_normal; /* 1 for Asset/Expense, 0 for Liability/Equity/Revenue*/
} Account;

/* Simple 1-Debit, 1-Credit Journal Entry*/
typedef struct {
    int month, day, year;
    char description[50];
    int debit_code;
    long long debit_amount;
    int credit_code;
    long long credit_amount;
} JournalEntry;

/* Main System Struct */
typedef struct {
    Account chart[9];
    JournalEntry entries[MAX_ENTRIES];
    int entry_count;
} BalanseKoSystem;

/* Function Prototypes*/
int read_int(const char *prompt, int min, int max); /* validated number input */
int read_yes_no(const char *prompt); /* returns 1 for y, 0 for n */   
void init_system(BalanseKoSystem *sys);
void show_chart(BalanseKoSystem *sys);
void add_entry(BalanseKoSystem *sys);
void manage_entries(BalanseKoSystem *sys); /* Edit / Delete menu */
void view_journal(BalanseKoSystem *sys);
void view_trial_balance(BalanseKoSystem *sys);

#endif