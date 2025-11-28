# **Personal Expense Tracker (Budget Manager)**

A console-based C application to record, manage, and analyze daily expenses using dynamic memory allocation and file handling.

## 📌 **About the Project**

This project helps users track their daily expenses with details like **date, category, amount, and notes**.
It uses **C programming fundamentals** such as:

* Structures
* Dynamic memory allocation
* File handling
* Modular functions
* String handling
* Menu-driven interaction

The program stores data permanently and loads it on startup, acting as a simple personal budgeting tool.

---

## 🚀 **Features**

### ✔ Add Expense

Enter date, category, amount, and optional notes.

### ✔ View All Expenses

Displays all recorded entries in a formatted table.

### ✔ Search Options

* Search by **category**
* Search by **date**

### ✔ Expense Summaries

* **Total spending**
* **Category-wise spending**

### ✔ Delete Expenses

Remove any expense using its unique ID.

### ✔ Auto Save & Load

* Saves all data to `expenses_db.txt`
* Loads automatically at program start

### ✔ Dynamic Memory Expansion

Grows as the number of expenses increases (using `malloc` & `realloc`).

---

## 🛠 **Technologies & Concepts Used**

* C language
* Structures
* Pointers
* Arrays & Strings
* Dynamic memory (`malloc`, `realloc`, `free`)
* File handling (`fopen`, fprintf, fscanf, fclose`)
* Modular programming
* Error handling

---

## 📂 **Project Structure**

```
📁 Expense-Tracker-C
│── expense_tracker.c
│── expenses_db.txt   (auto-created)
│── README.md
```

---

## 🖥️ **How to Run**

### **1. Compile the program**

```bash
gcc expense_tracker.c -o expense_tracker
```

### **2. Run the program**

```bash
./expense_tracker
```

On Windows:

```cmd
gcc expense_tracker.c -o expense_tracker.exe
expense_tracker.exe
```

---

## 📌 **Sample Output**

```
=== Personal Expense Tracker ===
1. Add Expense
2. Display All Expenses
3. Summary: Total Spent
4. Summary: By Category
5. Search By Category
6. Search By Date
7. Delete Expense
8. Save to File
9. Load from File
0. Exit
```

---

## 📊 **Data Storage Format**

Each line in `expenses_db.txt` uses this format:

```
ID|DATE|CATEGORY|AMOUNT|NOTE
```

Example:

```
1|2008-04-28|Uncategorized|2999|Birthday bill
```

---

## 🔮 **Future Improvements**

* GUI/Desktop version
* Password/login system
* Export to CSV/Excel
* Monthly graph-based analysis
* Cloud syncing
* Sorting (by amount/date/category)

---

## 👤 **Author**

**Parth Dogra**
UPES, Dehradun
Programming in C Project

---

## 📜 **License**

This project is for academic and personal use only.
Feel free to study, modify, and improve it.
