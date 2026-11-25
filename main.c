#include <stdio.h>
#include "balanseko.h"

/* Function Prototypes (all of them take the system by pointer = pass by reference) */
void print_main_menu(void);
void run_choice(BalanseKoSystem *sys, int choice);
void stay_in_option(BalanseKoSystem *sys, int choice);
void run_main_menu(BalanseKoSystem *sys);

int main() {
    BalanseKoSystem sys;

    init_system(&sys);
    run_main_menu(&sys);  /* The whole program runs inside this function */

    return 0;
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
    printf(" 0. Exit\n");
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
            printf("Thank you for using BalanseKo Services developed by Accounting Divas.\n");
        } else {
            stay_in_option(sys, choice);
        }
    }
}