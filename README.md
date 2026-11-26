# BalanseKo 

BalanseKo is a simple console-based bookkeeping program written in C. It lets a user record journal entries using a fixed chart of accounts, check them in the general journal and the trial balance, and view the Income Statement and Balance Sheet. Entries are saved to a file, so nothing is lost when the program closes.

## Features

- View the fixed Chart of Accounts (9 accounts)
- Add a journal entry (1 debit and 1 credit, which must be equal)
- Edit or delete an existing journal entry
- View the General Journal (shows account names instead of codes)
- View the Trial Balance
- View the Income Statement (revenue, expenses, net income or net loss)
- View the Balance Sheet (assets, liabilities, owner's equity)
- Entries are saved to `journal.txt` on exit and loaded when the program starts
- After each menu, the user is asked `Return to main menu? (y/n)`
- Input validation on every input (menu choices, date, account code, amount, description)
- ASCII art header

## Main Menu

```
1. View Chart of Accounts
2. Add Journal Entry
3. Edit / Delete Journal Entry
4. View General Journal
5. View Trial Balance
6. View Income Statement
7. View Balance Sheet
0. Save and Exit
```

## Project Files

| File | Purpose |
|---|---|
| `main.c` | ASCII header, main menu and the functions that run the menus |
| `balanseko.c` | All functions: input helpers, journal, edit/delete, trial balance, financial statements, save and load |
| `balanseko.h` | Structures, constants and function prototypes |
| `journal.txt` | Saved entries (created automatically when the program exits) |

## Input Format

- **Date:** `MM/DD/YYYY` (example: `03/15/2026`)
- **Amount:** a whole number from 1 up to 1,000,000,000,000,000 (no decimals)
- **Account code:** must be one of the codes shown in the chart

## Limitations of Prototype 2

- Amounts are whole numbers only, so centavos are not supported yet.
- Maximum of 200 entries, 1 debit and 1 credit per entry.
- Fixed chart of accounts (9 accounts).
