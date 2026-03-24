# 🌳 Multiset ADT (C)

A low-level data structures project written in C that implements a **Multiset Abstract Data Type (ADT)** using an **AVL tree** for efficient storage, balancing, and retrieval.

---

## 🚀 Key Highlights

- Implemented a complete **Multiset ADT in C**
- Used a **self-balancing AVL tree** to maintain efficient operations
- Supported both **basic multiset operations** and **advanced set-like operations**
- Built additional functionality such as **index-based access** and **cursor traversal**
- Demonstrates strong understanding of **data structures, memory management, and algorithmic efficiency**

---

## 📖 Overview

This project implements a **multiset** (also called a *bag*), a data structure that stores elements along with how many times each element appears.

Unlike a normal set, duplicate values are allowed. To keep operations efficient, the multiset is implemented using an **AVL tree**, a self-balancing binary search tree that ensures the structure remains balanced after insertions and deletions.

This allows common operations such as insertion, deletion, and lookup to run efficiently while keeping the data ordered.

---

## 🛠️ Technologies Used

- **Language:** C
- **Data Structure:** AVL Tree
- **Concepts:** ADT design, recursion, dynamic memory allocation, balancing, ordered traversal

---

## ▶️ How to Compile

```bash
make
./testMset
```

---

## 📦 How to Use the Multiset

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
| `MsetIncluded(s1, s2)` | Check if s1 is included in s2 | `MsetIncluded(s1, s2);` |
| `MsetEquals(s1, s2)` | Check if two multisets are equal | `MsetEquals(s1, s2);` |

---

### 🔹 Index Operations

| Function | Description | Example |
|---------|-------------|---------|
| `MsetAtIndex(s, index)` | Get element at a sorted index | `struct item x = MsetAtIndex(s, 0);` |
| `MsetIndexOf(s, elem)` | Get the sorted index of an element | `int i = MsetIndexOf(s, 5);` |

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

## 🧠 Technical Insights

- **AVL Tree Balancing**  
  The multiset uses an AVL tree, which keeps the structure balanced after insertions and deletions to support efficient operations.

- **Efficient Counting**  
  Each node stores both the element and its count, allowing duplicates to be handled cleanly without storing repeated nodes.

- **Memory Management in C**  
  The project uses manual dynamic memory allocation and cleanup, which demonstrates understanding of low-level resource management.

- **Ordered Traversal**  
  Since the multiset is stored in a balanced binary search tree, elements can be accessed in sorted order.

- **Extended Functionality**  
  Beyond basic insert/delete operations, the implementation also supports unions, intersections, cursor traversal, and index-based access.

---

## 💡 Why This Project Matters

This project demonstrates the ability to:

- implement a non-trivial abstract data type from scratch
- apply tree balancing algorithms to maintain efficiency
- manage memory safely in a low-level language
- design modular and reusable data structure code
- reason about algorithmic performance and ordered data access

It reflects strong foundations in **data structures, algorithms, and systems-level programming**, which are valuable in software engineering and backend development.

---

## 🚀 Author

**Kenneth Anthony Yusuf**  
Computer Science Student @ UNSW