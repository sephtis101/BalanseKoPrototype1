#include <stdio.h>
#include "balanseko.h"

/* Function Prototypes (all of them take the system by pointer = pass by reference) */
void choose_start(BalanseKoSystem *sys);
int load_previous(BalanseKoSystem *sys);
int start_new(BalanseKoSystem *sys);
void print_main_menu(void);
void run_choice(BalanseKoSystem *sys, int choice);
void stay_in_option(BalanseKoSystem *sys, int choice);
void run_main_menu(BalanseKoSystem *sys);

int main() {
    BalanseKoSystem sys;

    init_system(&sys);
    choose_start(&sys);   /* Ask: load a previous journal or start a new one */
    run_main_menu(&sys);  /* The whole program runs inside this function */

    return 0;
}

/* Option 1: shows the saved journals and loads the one the user picks.
   Returns 1 if a journal was loaded, 0 to go back to the start menu. */
int load_previous(BalanseKoSystem *sys) {
    int number;

    if (show_journals() == 0) {
        printf("No saved journals found.\n");
        return 0;
    }

    number = read_int("Journal number to load (0 to go back): ", 0, MAX_JOURNALS);
    while (number != 0 && load_data(sys, number) == 0) {
        printf("Journal %d was not found. Choose one from the list.\n", number);
        number = read_int("Journal number to load (0 to go back): ", 0, MAX_JOURNALS);
    }
    if (number == 0) return 0;

    printf("Journal %d loaded (%d entries).\n", number, sys->entry_count);
    return 1;
}

/* Option 2: starts an empty journal with the next free number.
   Returns 1 if it started, 0 if there is no free number left. */
int start_new(BalanseKoSystem *sys) {
    if (new_journal(sys) == 0) {
        printf("Journal limit reached (%d). Delete an old journal file first.\n", MAX_JOURNALS);
        return 0;
    }
    printf("Starting new journal %d (it will be saved as journal_%d.txt).\n",
           sys->journal_number, sys->journal_number);
    return 1;
}

/* Process: asks whether to load a previous journal or start a new one */
void choose_start(BalanseKoSystem *sys) {
    int started = 0;

    while (started == 0) {
        printf("\n--- START ---\n");
        printf(" 1. Load a previous journal\n");
        printf(" 2. Start a new journal\n");

        if (read_int("Choice: ", 1, 2) == 1) {
            started = load_previous(sys);
        } else {
            started = start_new(sys);
        }
    }
}

/* Output: shows the main menu */
void print_main_menu(void) {
    printf("\n==========================================================\n");
    printf("  ____          _                            _  __\n");
    printf(" | __ )   __ _ | |  __ _  _ __    ___   ___ | |/ /  ___\n");
    printf(" |  _ \\  / _` || | / _` || '_ \\  / __| / _ \\| ' /  / _ \\\n");
    printf(" | |_) || (_| || || (_| || | | | \\__ \\|  __/| . \\ | (_) |\n");
    printf(" |____/  \\__,_||_| \\__,_||_| |_| |___/ \\___||_|\\_\\ \\___/\n");
    printf("           Simple Journal & Bookkeeping System\n");
    printf("==========================================================\n");
    printf(" 1. View Chart of Accounts\n");
    printf(" 2. Add Journal Entry\n");
    printf(" 3. Edit / Delete Journal Entry\n");
    printf(" 4. View General Journal\n");
    printf(" 5. View Trial Balance\n");
    printf(" 6. View Income Statement\n");
    printf(" 7. View Balance Sheet\n");
    printf(" 0. Save and Exit\n");
    printf("----------------------------------------------------------\n");
}

/* Process: runs the chosen menu ONE time */
void run_choice(BalanseKoSystem *sys, int choice) {
    switch (choice) {
        case 1: show_chart(sys); break;
        case 2: add_entry(sys); break;
        case 3: manage_entries(sys); break;
        case 4: view_journal(sys); break;
        case 5: view_trial_balance(sys); break;
        case 6: view_income_statement(sys); break;
        case 7: view_balance_sheet(sys); break;
    }
}

/* Process: stays inside the chosen menu until the user agrees to go back */
void stay_in_option(BalanseKoSystem *sys, int choice) {
    int go_back = 0;

    while(go_back == 0) {
        run_choice(sys, choice);
        go_back = read_yes_no("\nReturn to main menu? (y/n): ");
    }
}

void run_main_menu(BalanseKoSystem *sys) {
    int choice = -1;

    while (choice != 0) {
        print_main_menu();
        choice = read_int("Choice: ", 0, 7);

        if (choice == 0) {
            save_data(sys);
            printf("Thank you for using BalanseKo Services developed by Accounting Divas.\n");
        } else {
            stay_in_option(sys, choice);
        }
    }
}