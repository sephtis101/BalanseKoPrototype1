# BalanseKo (Prototype 1)

BalanseKo is a simple console-based bookkeeping program written in C. It lets a user record journal entries using a fixed chart of accounts and then check them in the general journal and the trial balance.

## Features
- View the fixed Chart of Accounts (9 accounts)
- Add a journal entry (1 debit and 1 credit, which must be equal)
- Edit or delete an existing journal entry
- View the General Journal (shows account names instead of codes)
- View the Trial Balance
- Input validation on every input (menu choices, date, account code, amount, description)
- ASCII art header

## Main Menu

```
1. View Chart of Accounts
2. Add Journal Entry
3. Edit / Delete Journal Entry
4. View General Journal
5. View Trial Balance
0. Exit
```

## Project Files

| File | Purpose |
|---|---|
| `main.c` | Header design and the main menu loop |
| `balanseko.c` | All functions: input helpers, journal, edit/delete, trial balance |
| `balanseko.h` | Structures, constants and function prototypes |


## Input Format

- **Date:** `MM DD YYYY` (example: `03 15 2026`)
- **Amount:** a number greater than 0 (decimals allowed)
- **Account code:** must be one of the codes shown in the chart

## Limitations of Prototype 1

- Entries are kept in memory only, so everything is lost when the program exits.
- Maximum of 200 entries, 1 debit and 1 credit per entry.
- No financial statements, only the Trial Balance.
