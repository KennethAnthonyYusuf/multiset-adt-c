# Multiset ADT (C)

An implementation of a Multiset Abstract Data Type (ADT) in C using an AVL tree for efficient storage and operations.

---

## 📖 Overview

This project implements a multiset (also known as a bag), a data structure that allows multiple occurrences of the same element. 

The multiset is implemented using a **self-balancing AVL tree**, ensuring efficient operations such as insertion, deletion, and lookup in **O(log n)** time.

It also includes advanced features such as union, intersection, indexing, and cursor-based traversal.

---

## ✨ Features

- Insert single or multiple elements
- Delete single or multiple elements
- Count occurrences of elements
- Maintain sorted order automatically (AVL tree)
- Efficient operations (logarithmic time complexity)
- Union and intersection of multisets
- Equality and inclusion checks
- Access elements by index
- Cursor-based traversal (next/previous)
- Doubly linked structure for ordered iteration
- Automatic memory management

---

## 🛠️ Technologies Used

- **Language:** C  
- **Concepts:**  
  - Abstract Data Types (ADT)  
  - AVL Trees (self-balancing BST)  
  - Recursion  
  - Dynamic Memory Allocation (`malloc`, `free`)  
  - Structs and pointers  
  - Tree rotations (left/right)  
  - Doubly linked list (via `next` / `prev`)  

---

## ▶️ How to Compile

```bash
make
./mset
```

---

## How to Use the Multiset

The multiset is a library (ADT), so you use it by calling functions in your C program.

---

### 🔹 Basic Operations

| Function | Description | Example |
|---------|-------------|---------|
| `MsetNew()` | Create a new empty multiset | `Mset s = MsetNew();` |
| `MsetFree(s)` | Free all memory used by the multiset | `MsetFree(s);` |
| `MsetInsert(s, elem)` | Insert one occurrence of an element | `MsetInsert(s, 5);` |
| `MsetInsertMany(s, elem, amount)` | Insert multiple occurrences | `MsetInsertMany(s, 3, 2);` |
| `MsetDelete(s, elem)` | Delete one occurrence of an element | `MsetDelete(s, 5);` |
| `MsetDeleteMany(s, elem, amount)` | Delete multiple occurrences | `MsetDeleteMany(s, 3, 2);` |
| `MsetSize(s)` | Get number of distinct elements | `int n = MsetSize(s);` |
| `MsetTotalCount(s)` | Get total number of elements | `int t = MsetTotalCount(s);` |
| `MsetGetCount(s, elem)` | Get count of a specific element | `MsetGetCount(s, 3);` |
| `MsetPrint(s, file)` | Print multiset in sorted order | `MsetPrint(s, stdout);` |

---

### 🔹 Advanced Operations

| Function | Description | Example |
|---------|-------------|---------|
| `MsetUnion(s1, s2)` | Create union of two multisets | `Mset u = MsetUnion(s1, s2);` |
| `MsetIntersection(s1, s2)` | Create intersection of two multisets | `Mset i = MsetIntersection(s1, s2);` |
| `MsetIncluded(s1, s2)` | Check if s1 is subset of s2 | `MsetIncluded(s1, s2);` |
| `MsetEquals(s1, s2)` | Check if two multisets are equal | `MsetEquals(s1, s2);` |

---

### 🔹 Index Operations

| Function | Description | Example |
|---------|-------------|---------|
| `MsetAtIndex(s, index)` | Get element at index | `struct item x = MsetAtIndex(s, 0);` |
| `MsetIndexOf(s, elem)` | Get index of an element | `int i = MsetIndexOf(s, 5);` |

---

### 🔹 Cursor Operations

| Function | Description | Example |
|---------|-------------|---------|
| `MsetCursorNew(s)` | Create a cursor | `MsetCursor cur = MsetCursorNew(s);` |
| `MsetCursorGet(cur)` | Get current element | `struct item x = MsetCursorGet(cur);` |
| `MsetCursorNext(cur)` | Move to next element | `MsetCursorNext(cur);` |
| `MsetCursorPrev(cur)` | Move to previous element | `MsetCursorPrev(cur);` |
| `MsetCursorFree(cur)` | Free cursor memory | `MsetCursorFree(cur);` |

---