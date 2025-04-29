# hash-table-compiler

# Project 1 – Operating Systems: Binary Hash Table

---

## About the Project

This project was honestly one of the more challenging ones so far. The goal was to create a hash table and store it in a binary file using only system calls like `open()`, `read()`, `write()`, `lseek()`, and `close()`. No STL or fancy libraries allowed — everything had to be done manually, which made it a great learning experience.

The main idea was to store employee names and their salaries in a binary file and then be able to look up an employee’s salary using just their name (kind of like a tiny database).

---

## How It Works

- Reads from `employees.txt` — each line has a name and salary like `Mary,50000`
- Uses a custom hash function to assign that name to one of 200 buckets
- Writes each employee record (name + salary) to the correct spot in the binary file using `lseek()` and `write()`
- Handles collisions with **linear probing**
- Later, you can search for names (via command-line arguments), and it will print the salary and the index if found

---

## Hash Function

The hash function uses a simple polynomial approach with 31 as the multiplier — I read that 31 is a good prime number for hashing strings:

```cpp
int hash = 0;
for (int i = 0; name[i] != '\0'; ++i) {
    hash = (hash * 31 + name[i]) % 200;
}
```

---

## Files Included

- `palodkar_24612657.cpp` – main program file with all logic
- `employees.txt` – input file with name,salary values
- `palodkar_24612657.out` – output from test case 1
- `palodkar_24612657.2.out` – output from test case 2
- `employee_table.bin` – auto-generated binary file (you don’t need to upload this)

---

## How to Run It

First, compile it:

```bash
g++ palodkar_24612657.cpp -o palodkar_24612657.exe
```

Then run the test cases:

```bash
./palodkar_24612657.exe Christina Dhanesh Sylvester Jinming Juan Megan Rashad Stanley Wenxu Jackie > palodkar_24612657.out
./palodkar_24612657.exe Amire Daniel Derek Reginald Jaejun Vincenzo Rojan Sparky Emiliano Cesar > palodkar_24612657.2.out
```

You can check the output files to verify the results.

---

## What I Learned

- Working with `lseek()` and binary files was definitely tricky at first, especially figuring out how to jump to the right location
- I also learned how to build a basic hash table without any STL help, which was kind of cool
- This was the first time I had to think about file offsets and memory layout like this

---

## Notes

- Make sure salaries are between 10,000 and 250,000 (some test data had really high values, so I had to filter them out)
- Max name length is 16 characters
- All records are written as a fixed-size struct to the binary file

---

## Final Thoughts

This project was a good mix of systems programming and basic data structures. Took me a few tries to get everything working properly, especially the search part with collisions, but once it clicked it was satisfying.

---
