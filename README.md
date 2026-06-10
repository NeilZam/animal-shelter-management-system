# Animal Shelter Management System

A Binary Search Tree (BST) application written in C that manages cat records for an animal shelter. The program reads operations from an input file and supports inserting, deleting, searching, ranking, and filtering animals by traits using a fully dynamic BST with subtree metadata.

---

## How It Works

Each cat record stores a name, breed, charm score, and a set of 5 personality traits (friendly, grumpy, playful, lazy, curious). Records are organized in a BST keyed by name for efficient O(log n) search and retrieval. Every node tracks its subtree size, enabling O(log n) rank queries without a full traversal. If a duplicate name is inserted with more traits, the existing record is replaced.

---

## Getting Started

### Prerequisites

- GCC compiler
- Linux or Unix-based terminal (tested on Eustis)

### Compile

```bash
gcc -o shelter shelter.c
```

### Run

The program reads all operations from `tournament.txt` automatically:

```bash
./shelter
```

---

## Input File Format

The first line contains the number of operations. Each operation starts with a query number:

| Query | Operation |
|---|---|
| 1 | Insert a cat record |
| 2 | Delete a cat by name |
| 3 | Find the kth smallest by charm score |
| 4 | Filter cats by trait and print matches |
| 5 | Filter cats by trait and remove matches |
| 6 | Print all records in order |

**Example input (tournament.txt):**
```
6
1 Whiskers Tabby 42 1 0 1 0 1
1 Shadow Persian 35 0 1 0 1 0
3 1
4 0 1
2 Shadow
6
```

**Example output:**
```
Insert: 0
Insert: 1
Whiskers Tabby 42
friendly: Whiskers
Deletion Complete
Whiskers 42 1
```

---

## Features

- Insert cat records into a BST keyed by name
- Delete records using standard BST deletion with in-order successor
- Find the kth smallest element using subtree size metadata in O(log n)
- Filter cats by any of 5 personality traits
- Bulk delete all cats matching a trait
- Print all records in sorted order via in-order traversal
- Replace duplicate entries only if the new record has more active traits
- Full dynamic memory allocation and cleanup with no memory leaks

---

## Concepts Used

- Binary Search Trees
- Recursion
- Dynamic Memory Allocation
- Tree Traversal
- Subtree Metadata Tracking
- Algorithm Analysis

---

## Technologies

- C
- GCC
- Command Line

---

## Notes

- Cat names and breeds support up to 25 characters
- Supports up to 10,000 records
- Input files use simple 8-bit ASCII encoding
- All execution commands have been tested on Eustis

---

## What I Learned

This project strengthened my understanding of recursive data structures, memory management, and efficient searching algorithms. Implementing subtree size tracking for O(log n) rank queries was a key challenge that deepened my understanding of augmented BSTs.
