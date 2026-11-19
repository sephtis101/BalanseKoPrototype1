#include <stdio.h>
#include "balanseko.h"

/* Shows the BalanseKo header design */
void print_header(void) {
    printf("\n==========================================================\n");
    printf("  ____          _                            _  __\n");
    printf(" | __ )   __ _ | |  __ _  _ __    ___   ___ | |/ /  ___\n");
    printf(" |  _ \\  / _` || | / _` || '_ \\  / __| / _ \\| ' /  / _ \\\n");
    printf(" | |_) || (_| || || (_| || | | | \\__ \\|  __/| . \\ | (_) |\n");
    printf(" |____/  \\__,_||_| \\__,_||_| |_| |___/ \\___||_|\\_\\ \\___/\n");
    printf("           Simple Journal & Bookkeeping System\n");
    printf("==========================================================\n");
}

int main() {
    BalanseKoSystem sys;
    int choice;

    init_system(&sys);

    do {
        print_header();
        printf(" 1. View Chart of Accounts\n");
        printf(" 2. Add Journal Entry\n");
        printf(" 3. Edit / Delete Journal Entry\n");
        printf(" 4. View General Journal\n");
        printf(" 5. View Trial Balance\n");
        printf(" 0. Exit\n");
        printf("----------------------------------------------------------\n");
        choice = read_int("Choice: ", 0, 5);

        switch(choice) {
            case 1: show_chart(&sys); break;
            case 2: add_entry(&sys); break;
            case 3: manage_entries(&sys); break;
            case 4: view_journal(&sys); break;
            case 5: view_trial_balance(&sys); break;
            case 0: printf("Goodbye!\n"); break;
        }
    } while(choice != 0);

    return 0;
}
