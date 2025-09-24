# Mini SQL

Mini SQL is a simple C program that allows you to run basic SQL-like queries on a text-based database (`students.txt`).  
It supports `SELECT` queries with optional `WHERE` clauses.

---

## **Features**
- `SELECT *` to display all data.
- Select specific fields like `SELECT name, age FROM students;`.
- Filter data using `WHERE`, e.g., `SELECT name, grade FROM students WHERE age=20;`.
- Exit the program using `EXIT;`.

---

## **How to Run**
1. Compile the program:
   ```bash
   gcc code.c -o mini-sql
   ```
2. Run it:
   ```bash
   ./mini-sql
   ```

For Windows (Git Bash):
```bash
gcc code.c
./a.exe
```

---

## **Example**
```
> SELECT * FROM students;
id    name    age    grade
1     Rahim   20     A
2     Karim   21     B
```

---

## **Git Ignore Rules**
```
*.exe
*.o
.vscode/
.vs/
*.zip
```

---

**Developer:** MD Razwanul Islam  
Begum Rokeya University, Rangpur
16th Batch