#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef _WIN32
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#define MAX_CAT_LEN 30
#define MAX_NOTE_LEN 100
#define INITIAL_CAPACITY 8
#define DATA_FILE "expenses_db.txt"

typedef struct {
    int id;
    char date[11];
    char category[MAX_CAT_LEN];
    double amount;
    char note[MAX_NOTE_LEN];
} Expense;

typedef struct {
    Expense *items;
    int size;
    int capacity;
    int next_id;
} ExpenseList;

void trim_newline_and_spaces(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || isspace((unsigned char)s[len-1]))) {
        s[len-1] = '\0';
        len--;
    }
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);
}

int read_line(char *buf, size_t bufsize) {
    if (!fgets(buf, (int)bufsize, stdin)) return 0;
    trim_newline_and_spaces(buf);
    return 1;
}

void initList(ExpenseList *list) {
    list->items = malloc(sizeof(Expense) * INITIAL_CAPACITY);
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    list->next_id = 1;
}

void freeList(ExpenseList *list) {
    if (list->items) free(list->items);
}

void ensureCapacity(ExpenseList *list) {
    if (list->size >= list->capacity) {
        int newcap = list->capacity * 2;
        Expense *tmp = realloc(list->items, newcap * sizeof(Expense));
        if (!tmp) return;
        list->items = tmp;
        list->capacity = newcap;
    }
}

void displayExpense(const Expense *e) {
    printf("%-4d %-10s %-15s %10.2f  %s\n",
           e->id, e->date, e->category, e->amount, e->note);
}

void addExpense(ExpenseList *list) {
    Expense e;
    e.id = list->next_id++;

    char line[256];

    printf("Enter date (YYYY-MM-DD): ");
    if (!read_line(e.date, sizeof(e.date))) return;
    if (strlen(e.date) == 0) strcpy(e.date, "0000-00-00");

    printf("Enter category: ");
    if (!read_line(e.category, sizeof(e.category))) return;
    if (strlen(e.category) == 0) strcpy(e.category, "Uncategorized");

    double amt = 0.0;
    while (1) {
        printf("Enter amount: ");
        if (!read_line(line, sizeof(line))) return;
        if (line[0] == '\0') continue;
        char *endptr = NULL;
        errno = 0;
        amt = strtod(line, &endptr);
        if (endptr == line) continue;
        if (errno == ERANGE) continue;
        if (amt < 0.0) continue;
        break;
    }
    e.amount = amt;

    printf("Enter note (optional): ");
    if (!read_line(e.note, sizeof(e.note))) e.note[0] = '\0';

    ensureCapacity(list);
    list->items[list->size++] = e;
    printf("\nExpense added with ID %d\n", e.id);
}

void displayAll(const ExpenseList *list) {
    if (list->size == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    printf("\nID   DATE       CATEGORY        AMOUNT      NOTE\n");
    printf("---- ---------- --------------- ----------  --------------------------\n");
    for (int i = 0; i < list->size; ++i) {
        displayExpense(&list->items[i]);
    }
}

void summaryTotal(const ExpenseList *list) {
    double total = 0.0;
    for (int i = 0; i < list->size; ++i) total += list->items[i].amount;
    printf("Total amount spent: %.2f\n", total);
}

void summaryByCategory(const ExpenseList *list) {
    if (list->size == 0) {
        printf("No expenses recorded.\n");
        return;
    }

    #define MAX_CATS 200
    char cats[MAX_CATS][MAX_CAT_LEN];
    double sums[MAX_CATS];
    int catCount = 0;

    for (int i = 0; i < list->size; ++i) {
        int found = -1;
        for (int j = 0; j < catCount; ++j) {
            if (strcasecmp(cats[j], list->items[i].category) == 0) {
                found = j;
                break;
            }
        }
        if (found == -1) {
            strncpy(cats[catCount], list->items[i].category, MAX_CAT_LEN);
            cats[catCount][MAX_CAT_LEN-1] = '\0';
            sums[catCount] = list->items[i].amount;
            catCount++;
        } else {
            sums[found] += list->items[i].amount;
        }
    }

    printf("\nCATEGORY         TOTAL\n");
    printf("------------------------\n");
    for (int i = 0; i < catCount; ++i) {
        printf("%-15s %10.2f\n", cats[i], sums[i]);
    }
}

void searchByCategory(const ExpenseList *list) {
    char cat[MAX_CAT_LEN];
    printf("Enter category to search: ");
    if (!read_line(cat, sizeof(cat))) return;
    if (strlen(cat) == 0) return;

    int found = 0;
    for (int i = 0; i < list->size; ++i) {
        if (strcasecmp(list->items[i].category, cat) == 0) {
            if (!found) {
                printf("\nID   DATE       CATEGORY        AMOUNT      NOTE\n");
                printf("---- ---------- --------------- ----------  --------------------------\n");
            }
            displayExpense(&list->items[i]);
            found++;
        }
    }
    if (!found) printf("No expenses found for category '%s'\n", cat);
}

void searchByDate(const ExpenseList *list) {
    char date[11];
    printf("Enter date (YYYY-MM-DD): ");
    if (!read_line(date, sizeof(date))) return;
    if (strlen(date) == 0) return;

    int found = 0;
    for (int i = 0; i < list->size; ++i) {
        if (strcmp(list->items[i].date, date) == 0) {
            if (!found) {
                printf("\nID   DATE       CATEGORY        AMOUNT      NOTE\n");
                printf("---- ---------- --------------- ----------  --------------------------\n");
            }
            displayExpense(&list->items[i]);
            found++;
        }
    }
    if (!found) printf("No expenses found for date '%s'\n", date);
}

void deleteExpense(ExpenseList *list) {
    if (list->size == 0) {
        printf("No expenses to delete.\n");
        return;
    }
    char line[64];
    printf("Enter expense ID: ");
    if (!read_line(line, sizeof(line))) return;
    int id = atoi(line);
    if (id <= 0) return;

    int idx = -1;
    for (int i = 0; i < list->size; ++i) {
        if (list->items[i].id == id) { idx = i; break; }
    }
    if (idx == -1) {
        printf("ID not found.\n");
        return;
    }

    list->items[idx] = list->items[list->size - 1];
    list->size--;
    printf("Expense deleted.\n");
}

void saveToFile(const ExpenseList *list) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) return;
    fprintf(fp, "%d\n", list->next_id);
    for (int i = 0; i < list->size; ++i) {
        fprintf(fp, "%d|%s|%s|%.2f|%s\n",
                list->items[i].id,
                list->items[i].date,
                list->items[i].category,
                list->items[i].amount,
                list->items[i].note);
    }
    fclose(fp);
}

void loadFromFile(ExpenseList *list) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) return;

    char line[512];

    if (fgets(line, sizeof(line), fp)) {
        trim_newline_and_spaces(line);
        int nid = atoi(line);
        if (nid > 0) list->next_id = nid;
    }

    while (fgets(line, sizeof(line), fp)) {
        trim_newline_and_spaces(line);
        if (line[0] == '\0') continue;

        Expense e;
        char *p = strtok(line, "|");
        if (!p) continue;
        e.id = atoi(p);

        p = strtok(NULL, "|");
        if (!p) continue;
        strncpy(e.date, p, sizeof(e.date));

        p = strtok(NULL, "|");
        if (!p) continue;
        strncpy(e.category, p, sizeof(e.category));

        p = strtok(NULL, "|");
        if (!p) continue;
        e.amount = atof(p);

        p = strtok(NULL, "|");
        if (!p) e.note[0] = '\0';
        else strncpy(e.note, p, sizeof(e.note));

        ensureCapacity(list);
        list->items[list->size++] = e;
        if (e.id >= list->next_id) list->next_id = e.id + 1;
    }

    fclose(fp);
}

void pause_console() {
    char tmp[8];
    printf("\nPress Enter to continue...");
    read_line(tmp, sizeof(tmp));
}

int menu() {
    printf("\n=== Personal Expense Tracker ===\n");
    printf("1. Add Expense\n");
    printf("2. Display All Expenses\n");
    printf("3. Summary: Total Spent\n");
    printf("4. Summary: By Category\n");
    printf("5. Search By Category\n");
    printf("6. Search By Date\n");
    printf("7. Delete Expense\n");
    printf("8. Save to File\n");
    printf("9. Load from File\n");
    printf("0. Exit\n");
    printf("Choose option: ");
    char line[16];
    if (!read_line(line, sizeof(line))) return -1;
    return atoi(line);
}

int main() {
    ExpenseList list;
    initList(&list);
    loadFromFile(&list);

    while (1) {
        int choice = menu();
        switch (choice) {
            case 1: addExpense(&list); pause_console(); break;
            case 2: displayAll(&list); pause_console(); break;
            case 3: summaryTotal(&list); pause_console(); break;
            case 4: summaryByCategory(&list); pause_console(); break;
            case 5: searchByCategory(&list); pause_console(); break;
            case 6: searchByDate(&list); pause_console(); break;
            case 7: deleteExpense(&list); pause_console(); break;
            case 8: saveToFile(&list); pause_console(); break;
            case 9: loadFromFile(&list); pause_console(); break;
            case 0:
                saveToFile(&list);
                freeList(&list);
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid option.\n");
                pause_console();
                break;
        }
    }
}
