// Implementation of the Multiset ADT
// By Kenneth Anthony Yusuf

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"

static void NodeFree(struct node *n);
static struct node *NodeInsert(Mset s, struct node *n, int elem);
static struct node *NodeInsertMany(Mset s, struct node *n, int elem, 
	int amount);
static struct node *NodeDelete(Mset s, struct node *n, int elem);
static struct node *JoinNode(struct node *n);
static struct node *NodeDeleteMany(Mset s, struct node *n, int elem, 
	int amount);
static int NodeFindCount(struct node *n, int elem);
static void InOrderPrint(struct node *n, FILE *file, int *num);
static void CreateCopy(Mset new, struct node *t1);
static void AddUnion(Mset new, struct node *t2);
static void AddIntersection(Mset new, Mset s2, struct node *t);
static bool CheckIncludedNodes(struct node *t1, Mset s2);
static bool CheckEqualsNode(Mset s2, struct node *t1);
static struct node *avlRebalance(struct node *t);
static int Balance(struct node *t);
static int Height(struct node *t);
static struct node *RotateLeft(struct node *oldRoot);
static struct node *RotateRight(struct node *oldRoot);
static void HeightManagement(struct node *t);
static int HeightCheck(int right, int left);
static struct node *FindJoinMin(struct node *curr, struct node **min);
static void SizeManagement(struct node *t);
static int GetSize(struct node *t);
struct node *NodeAtIndex(struct node *t, int index);
static int FindIndex(struct node *t, int elem, int index);
static void OrderManagement(Mset s, struct node *new);
static void CursorCheck(struct node *t, MsetCursor cursor);

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void) {
	Mset s = malloc(sizeof(*s));

	if (s == NULL) {
		fprintf(stderr, "error: malloc failed\n");
		exit(1);
	}
	
	s->first = NULL;
	s->last = NULL;
	s->tree = NULL;
	s->size = 0;
	s->totalCount = 0;
	s->cursor = NULL;

	return s;
}

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s) {
	NodeFree(s->tree);
	s->tree = NULL;
	free(s);
}

// Traverse through all the tree node and free them one by one.
static void NodeFree(struct node *n) {
	if(n == NULL) {
		return;
	}

	NodeFree(n->left);
	NodeFree(n->right);
	free(n);
}

/**
 * Inserts one of an element into the multiset. Does nothing if the
 * element is equal to UNDEFINED.
 */
void MsetInsert(Mset s, int elem) {
	if (elem == UNDEFINED) {
		return;
	} 

	s->tree = NodeInsert(s, s->tree, elem);
}

// Traverse through the tree to insert the given element.
// Return a new root for the multiset tree containing the new element.
static struct node *NodeInsert(Mset s, struct node *t, int elem) {
	// Return the new node if t hit NULL.
	if (t == NULL) {
		struct node *new = malloc(sizeof(*new));

		if (new == NULL) {
			fprintf(stderr, "error: malloc failed\n");
			exit(1);
		}

		// Initialize all the new node field.
		new->elem = elem;
		new->count = 1;
		new->left = NULL;
		new->right = NULL;
		new->height = 0;
		new->size = 1;
		new->next = NULL;
		new->prev = NULL;
		s->size = s->size + 1;
		s->totalCount = s->totalCount + 1;

		// Finding the next and prev pointer for the new node.
		OrderManagement(s, new);

		return new;
	}

	// Traverse through the BST
	if (elem > t->elem) {
		t->right = NodeInsert(s, t->right, elem);
	} else if (elem < t->elem) {
		t->left = NodeInsert(s, t->left, elem);
	} else if (elem == t->elem) {
		// Updating the current element count an the global count.
		t->count = t->count + 1;
		s->totalCount = s->totalCount + 1;

		return t;
	}

	// Updating the size of node t.
	SizeManagement(t);

	// Updating the heigh of node t.
	HeightManagement(t);

	// Adjusting the balance for node t.
	t = avlRebalance(t);

	return t;
}

/**
 * Inserts the given amount of an element into the multiset. Does
 * nothing if the element is equal to UNDEFINED or the given amount is 0
 * or less.
 */
void MsetInsertMany(Mset s, int elem, int amount) {
	if (elem == UNDEFINED) {
		return;
	} else if (amount <= 0) {
		return;
	}

	s->tree = NodeInsertMany(s, s->tree, elem, amount);
}

// Traverse through the tree to insert multiple elements.
// Return a new root for the multiset tree containing the new elements.
static struct node *NodeInsertMany(Mset s, struct node *t, int elem, 
	int amount) {
	// Return the new node if t hit NULL.
	if (t == NULL) {
		struct node *new = malloc(sizeof(*new));

		if (new == NULL) {
			fprintf(stderr, "error: malloc failed\n");
			exit(1);
		}

		// Initialize all the new node field.
		new->elem = elem;
		new->count = amount;
		new->left = NULL;
		new->right = NULL;
		new->height = 0;
		new->size = 1;
		new->prev = NULL;
		new->next = NULL;
		s->size = s->size + 1;
		s->totalCount = s->totalCount + amount;

		// Finding the next and prev pointer for the new node.
		OrderManagement(s, new);

		return new;
	}

	// Traverse through the BST
	if (elem > t->elem) {
		t->right = NodeInsertMany(s, t->right, elem, amount);
	} else if (elem < t->elem) {
		t->left = NodeInsertMany(s, t->left, elem, amount);
	} else if (elem == t->elem) {
		// Updating the current element count an the global count.
		t->count = t->count + amount;
		s->totalCount = s->totalCount + amount;

		return t;
	}

	// Updating the size of node t.
	SizeManagement(t);

	// Updating the heigh of node t.
	HeightManagement(t);

	// Adjusting the balance for node t.
	t = avlRebalance(t);

	return t;
}

/**
 * Deletes one of an element from the multiset.
 */
void MsetDelete(Mset s, int elem) {
	if (elem == UNDEFINED) {
		return;
	} 

	s->tree = NodeDelete(s, s->tree, elem);
}

// Traverse through the tree to delete the given element.
// Return a new root for the multiset tree with some deleted element.
static struct node *NodeDelete(Mset s, struct node *t, int elem) {
	if (t == NULL) {
		return NULL;
	}

	// Traverse through the BST
	if (elem > t->elem) {
		t->right = NodeDelete(s, t->right, elem);
	} else if (elem < t->elem) {
		t->left = NodeDelete(s, t->left, elem);
	} else if (elem == t->elem) {
		// Updating the global count.
		int totalCount = s->totalCount - 1;

		if (totalCount >= 0) {
			s->totalCount = totalCount;
		} else {
			s->totalCount = 0;
		}

		// Updating the current element count.
		int count = t->count - 1;
		
		if (count > 0) {
			t->count = count;
			return t;
		} 

		// Updating the tree size.
		if (s->size > 0) {
			s->size = s->size - 1;
		} 

		// Managing the cursor function validity.
		if (s->cursor != NULL) {
			CursorCheck(t, s->cursor);
		}

		struct node *right = t->right;
		struct node *left = t->left;

		struct node *before = t->prev;
		struct node *after = t->next;

		// Updating "prev" and "next" field for the node before
		// and after the current node.
		if (before != NULL) {
			before->next = after;
		} else {
			s->first = after;
		}
		if (after != NULL) {
			after->prev = before;
		} else {
			s->last = before;
		}

		// Freeing the given node and returning the replacement node.
		if (right == NULL) {
			free(t);
			return left;
		} else if (left == NULL) {
			free(t);
			return right;
		} else if (right != NULL && left != NULL) {
			// Joining both left and right child and returning the root.
			return JoinNode(t);
		} else {
			free(t);
			return NULL;
		}
	}

	// Updating the size of node t.
	SizeManagement(t);

	// Updating the heigh of node t.
	HeightManagement(t);

	// Adjusting the balance for node t.
	t = avlRebalance(t);

	return t;
}

// Joining both left and right child nodes of the current node.
// Return the root of two child nodes after joining them.
static struct node *JoinNode(struct node *t) {
	struct node *min = NULL;
	struct node *right = FindJoinMin(t->right, &min);

	// Joining the new root to both child nodes.
	min->right = right;
	min->left = t->left;

	free(t);

	// Updating the size of node t.
	SizeManagement(t);

	// Updating the heigh of node t.
	HeightManagement(t);

	return avlRebalance(min);
}

// Find the new root and remove the root from the right tree.
// Update the balance, height, and size of the right tree.
// Return the updated right tree.
static struct node *FindJoinMin(struct node *curr, struct node **min) {
	if (curr->left == NULL) {
		// New root.
		*min = curr;

		// Return the right child to detached the new root.
		return curr->right;
	}

	// Loop to find the new root.
	curr->left = FindJoinMin(curr->left, min);

	// Updating the size of node curr.
	SizeManagement(curr);

	// Updating the heigh of node curr.
	HeightManagement(curr);

	return avlRebalance(curr);
}

/**
 * Deletes the given amount of an element from the multiset.
 */
void MsetDeleteMany(Mset s, int elem, int amount) {
	if (elem == UNDEFINED) {
		return;
	} else if (amount <= 0) {
		return;
	}

	s->tree = NodeDeleteMany(s, s->tree, elem, amount);
}

// Traverse through the tree to delete the given elements.
// Return a new root for the multiset tree with some deleted elements.
static struct node *NodeDeleteMany(Mset s, struct node *t, 
	int elem, int amount) {
	if (t == NULL) {
		return NULL;
	}

	// Traverse through the BST.
	if (elem > t->elem) {
		t->right = NodeDeleteMany(s, t->right, elem, amount);
	} else if (elem < t->elem) {
		t->left = NodeDeleteMany(s, t->left, elem, amount);
	} else if (elem == t->elem) {
		// Updating the total local and global count.
		int count = t->count - amount;

		if (count > 0) {
			s->totalCount = s->totalCount - amount;
			t->count = count;
			return t;
		} 

		s->totalCount = s->totalCount - t->count;

		// Updating the tree size.
		if (s->size > 0) {
			s->size = s->size - 1;
		}

		// Managing the cursor function validity.
		if (s->cursor != NULL) {
			CursorCheck(t, s->cursor);
		}

		struct node *right = t->right;
		struct node *left = t->left;

		struct node *before = t->prev;
		struct node *after = t->next;

		// Updating "prev" and "next" field for the node before
		// and after the current node.
		if (before != NULL) {
			before->next = after;
		} else {
			s->first = after;
		}
		if (after != NULL) {
			after->prev = before;
		} else {
			s->last = before;
		}

		// Freeing the current node and returning the replacement node.
		if (right == NULL) {
			free(t);
			return left;
		} else if (left == NULL) {
			free(t);
			return right;
		} else if (right != NULL && left != NULL) {
			// Joining both left and right child and returning the root.
			return JoinNode(t);
		} else {
			free(t);
			return NULL;
		}
	}

	// Updating the size of node t.
	SizeManagement(t);

	// Updating the heigh of node t.
	HeightManagement(t);

	// Adjusting the balance for node t.
	t = avlRebalance(t);

	return t;
}

/**
 * Returns the number of distinct elements in the multiset.
 */
int MsetSize(Mset s) {
	return s->size;
}

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s) {
	return s->totalCount;
}

/**
 * Returns the count of an element in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int elem) {
	if (elem == UNDEFINED) {
		return 0;
	}

	return NodeFindCount(s->tree, elem);
}

// Traverse the tree to find the given element.
// Return the total count of the given element.
static int NodeFindCount(struct node *n, int elem) {
	if (n == NULL) {
		return 0;
	}

	int num = 0;

	if (elem > n->elem) {
		num = NodeFindCount(n->right, elem);
	} else if (elem < n->elem) {
		num = NodeFindCount(n->left, elem);
	} else {
		return n->count;
	}

	return num;
}

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 */
void MsetPrint(Mset s, FILE *file) {
	if (file == NULL) {
		return;
	}

	int one = 1;
	int *num = &one;

	fprintf(file, "{");
	InOrderPrint(s->tree, file, num);
	fprintf(file, "}");
}

// Traverse through the tree in-order.
// Print the element and count of each node in the tree by order.
static void InOrderPrint(struct node *n, FILE *file, int *num) {
	if (n == NULL) {
		return;
	}
 
	InOrderPrint(n->left, file, num);

	if (*num == 0) {
		fprintf(file, ", ");
	} else {
		*num = 0;
	}

	fprintf(file, "(%d, %d)", n->elem, n->count);
	InOrderPrint(n->right, file, num);
}

////////////////////////////////////////////////////////////////////////
// Balance BST Helper Function

// Check and update the balance of given tree.
// Return the root of the balance tree.
static struct node *avlRebalance(struct node *t) {
	int bal = Balance(t);

	if (bal < -1) {
		bal = Balance(t->right);
		if (bal > 0) {
			t->right = RotateRight(t->right);
		}
		t = RotateLeft(t);
	} else if (bal > 1) {
		bal = Balance(t->left);
		if (bal < 0) {
			t->left = RotateLeft(t->left);
		}
		t = RotateRight(t);
	} 
	
	return t;
}

// Return the height balance of the given tree.
static int Balance(struct node *t) {
	int balance = Height(t->left) - Height(t->right);

	return balance;
}

// Return the height of the given tree.
static int Height(struct node *t) {
	if (t == NULL) {
		return -1;
	}

	return t->height;
}

// Rotate left the given tree.
// Return the new root after the left rotation.
static struct node *RotateLeft(struct node *oldRoot) {
	if (oldRoot == NULL || oldRoot->right == NULL) {
		return oldRoot;
	}

	// Adjusting the tree.
	struct node *newRoot = oldRoot->right;
	oldRoot->right = newRoot->left;
	newRoot->left = oldRoot;

	// Updating the size of the old and new root.
	SizeManagement(oldRoot);
	SizeManagement(newRoot);

	// Updating the height of the old and new root.
	HeightManagement(oldRoot);
	HeightManagement(newRoot);

	return newRoot;
}

// Rotate right the given tree.
// Return the new root after the right rotation.
static struct node *RotateRight(struct node *oldRoot) {
	if (oldRoot == NULL || oldRoot->left == NULL) {
		return oldRoot;
	}

	// Adjusting the tree.
	struct node *newRoot = oldRoot->left;
	oldRoot->left = newRoot->right;
	newRoot->right = oldRoot;

	// Updating the size of the old and new root.
	SizeManagement(oldRoot);
	SizeManagement(newRoot);

	// Updating the height of the old and new root.
	HeightManagement(oldRoot);
	HeightManagement(newRoot);

	return newRoot;
}

// Updating the height of the given tree.
static void HeightManagement(struct node *t) {
	if (t == NULL) {
		return;
	}

	t->height = HeightCheck(Height(t->right), Height(t->left));
}

// Calculating the height from the left and right child height.
// Return the height.
static int HeightCheck(int right, int left) {
	if (right > left) {
		return right + 1;
	} else {
		return left + 1;
	}
}

// Updating the size of the given tree.
static void SizeManagement(struct node *t) {
	if (t == NULL) {
		return;
	}

	t->size = 1 + GetSize(t->left) + GetSize(t->right);
}

// Return the size of the given tree.
static int GetSize(struct node *t) {
	if (t == NULL) {
		return 0;
	}

	return t->size;
}

// Adjusting the "next" and "prev" field of the given node.
// Determine the "first" and "last" node of the multiset.
static void OrderManagement(Mset s, struct node *new) {
	struct node *curr = s->tree;
	struct node *prev = NULL;
	struct node *next = NULL;

	if (curr == NULL) {
		s->first = new;
		s->last = new;
		return;
	}

	// Traverse through the BST to find the prev and next node.
	while (curr != NULL) {
		if (new->elem < curr->elem) {
			next = curr;
			curr = curr->left;
		} else if (new->elem > curr->elem) {
			prev = curr;
			curr = curr->right;
		} else {
			return;
		}
	}

	// Updating the "next" and "prev" field of the given node.
	new->next = next;
	new->prev = prev;

	// Updating the "next" and "prev" field of the node before and 
	// after the given node.
	if (prev != NULL) {
		prev->next = new;
	} else {
		// Determine the "first" node of the multiset.
		s->first = new;
	}

	if (next != NULL) {
		next->prev = new;
	} else {
		// Determine the "last" node of the multiset.
		s->last = new;
	}
}

// Cheking the validity of cursor function.
static void CursorCheck(struct node *t, MsetCursor cursor) {
	struct item test = MsetCursorGet(cursor);
	if (test.elem == t->elem) {
		cursor->validity = false;
	}
}

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets.
 */
Mset MsetUnion(Mset s1, Mset s2) {
	// Create a new multiset.
	Mset new = MsetNew();

	// Copy the tree from multiset s1 into the new multiset.
	CreateCopy(new, s1->tree);

	// Add the tree union from multiset s2 into the new multiset.
	AddUnion(new, s2->tree);

	return new;
}

static void CreateCopy(Mset new, struct node *t1) {
	if (t1 == NULL) {
		return;
	}

	// Copy all the tree node from multiset s1 into the new multiset.
	MsetInsertMany(new, t1->elem, t1->count);
	
	// Traverse the tree t1 from multiset s1 by pre-order.
	CreateCopy(new, t1->left); 
	CreateCopy(new, t1->right); 
}

static void AddUnion(Mset new, struct node *t2) {
	if (t2 == NULL) {
		return;
	}

	// Traverse the tree t2 from multiset s2 by in-order.
	AddUnion(new, t2->left);

	// Get the total count of element t2 from the new multiset.
	int count = MsetGetCount(new, t2->elem);

	// Condition to add one or more elements from multiset s2 into 
	// the new multiset.
	if (t2->count > count) {
		MsetInsertMany(new, t2->elem, t2->count - count);
	}

	AddUnion(new, t2->right);
}

/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2) {
	// Create a new multiset.
	Mset new = MsetNew();

	// Add the tree intersection from multiset s2 into the new multiset.
	AddIntersection(new, s2, s1->tree);

	return new;
}

static void AddIntersection(Mset new, Mset s2, struct node *t1) {
	if (t1 == NULL) {
		return;
	}

	// Traverse the tree t1 from multiset s1 by post-order.
	AddIntersection(new, s2, t1->left);

	// Get the total count of element t1 from the multiset s2.
	int count = MsetGetCount(s2, t1->elem);

	// Condition to insert one or more elements from multiset s1
	// into the new multiset.
	if (count > 0 && t1->count > count) {
		MsetInsertMany(new, t1->elem, count);
	} else if (count > 0 && t1->count < count) {
		MsetInsertMany(new, t1->elem, t1->count);
	}

	AddIntersection(new, s2, t1->right);
}

/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
bool MsetIncluded(Mset s1, Mset s2) {
	// Call a function to check if multiset s1 is included in 
	// multiset s2.
	return CheckIncludedNodes(s1->tree, s2);
}

static bool CheckIncludedNodes(struct node *t1, Mset s2) {
	if (t1 == NULL) {
		return true;
	}

	// Traverse the tree t1 from multiset s1 by in-order.
	if (CheckIncludedNodes(t1->left, s2) == false) {
		return false;
	}

	// Get the total count of element t1 from the multiset s2.
	int count = MsetGetCount(s2, t1->elem);

	// Check if the element in t1 included in t2.
	if (count < t1->count) {
		return false;
	} 

	return CheckIncludedNodes(t1->right, s2);
}

/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
bool MsetEquals(Mset s1, Mset s2) {
	int size1 = MsetSize(s1);
	int size2 = MsetSize(s2);

	// Check if the tree size from multiset s1 and s2 are equal.
	if (size1 != size2) {
		return false;
	}

	return CheckEqualsNode(s2, s1->tree);
}

static bool CheckEqualsNode(Mset s2, struct node *t1) {
	if (t1 == NULL) {
		return true;
	}

	// Traverse trough tree t1 by in-order.
	if (CheckEqualsNode(s2, t1->left) == false) {
		return false;
	}

	// Get the total count of element t1 from the multiset s2.
	int count = MsetGetCount(s2, t1->elem);

	// Check if the count of a element from multiset s1 
	// and s2 are equal.
	if (count != t1->count) {
		return false;
	}

	return CheckEqualsNode(s2, t1->right);
}

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index and its count, or
 * {UNDEFINED, 0} if the given index is outside the range [0, n - 1]
 * where n is the size of the multiset.
 */
struct item MsetAtIndex(Mset s, int index) {
	if (index < 0 || index >= s->size ) {
		return (struct item){UNDEFINED, 0};
	}

	struct node *result = NodeAtIndex(s->tree, index);

	if (result == NULL) {
		return (struct item){UNDEFINED, 0};
	}

	return (struct item){result->elem, result->count};
}

// Traverse through the BST to find the node at the given index.
// Return the node at the given index.
struct node *NodeAtIndex(struct node *t, int index) {
	if (t == NULL) {
		return NULL;
	}

	struct node *result = NULL;
	int leftSize = GetSize(t->left);

	// Finding the element using node size.
	if (index < leftSize) {
		result = NodeAtIndex(t->left, index);
	} else if (index > leftSize) {
		result = NodeAtIndex(t->right, index - leftSize - 1);
	} else {
		return t;
	}

	return result;
}

/**
 * Returns the index of the given element if it exists, or -1 otherwise.
 */
int MsetIndexOf(Mset s, int elem) {

	return FindIndex(s->tree, elem, 0);
}

// Traverse through the BST to find the index of the given element.
// Return the index of the given element.
static int FindIndex(struct node *t, int elem, int past) {
	if (t == NULL) {
		return -1;
	}

	int indexLeft = GetSize(t->left);

	// Traverse through the BST to find the element and determine the 
	// index of the element.
	if (elem < t->elem) {
		return FindIndex(t->left, elem, past);
	} else if (elem > t->elem) {
		return FindIndex(t->right, elem, past + indexLeft + 1);
	} else {
		return past + indexLeft;
	}

	return -1;
} 

////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s) {
	MsetCursor cur = malloc(sizeof(*cur));

	if (cur == NULL) {
		fprintf(stderr,"error: malloc failed\n");
		exit(1);
	}

	// Initialize the cursor struct.
	cur->mset = s;
	cur->now = NULL;
	cur->start = true;
	cur->end = false;
	cur->validity = true;
	s->cursor = cur;

	return cur;
}

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur) {
	free(cur);
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	if (cur->start == true || cur->end == true || cur->now == NULL || 
		cur->validity == false) {
		return (struct item){UNDEFINED, 0};
	}

	return (struct item){cur->now->elem, cur->now->count};
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	// Checking if function is valid.
	if (cur->validity == false) {
		return false;
	} 

	// Check if the cursor is at the end of the list.
	else if (cur->end == true) {
		return false;
	} 
	
	// Check if the node with the smallest element exist.
	else if (cur->mset->first == NULL) {
		cur->start = false;
		cur->end = true;
		return false;
	} 
	
	// Check if the cursor pointing to a node with the largest element.
	else if (cur->now == cur->mset->last) {
		cur->end = true;
		cur->start = false;
		cur->now = NULL;
		return false;
	} 
	
	// Check the if the location of the cursor is at the start.
	else if (cur->start == true) {
		cur->now = cur->mset->first;
		cur->start = false;
		cur->end = false;
	} 
	
	// move the cursor to the next element.
	else {
		cur->now = cur->now->next;
		cur->start = false;
	}

	return true;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	// Checking if function is valid.
	if (cur->validity == false) {
		return false;
	} 
	
	// Check if the cursor is at the start of the list.
	else if (cur->start == true) {
		return false;
	} 
	
	// Check if the node with the largest element exist.
	else if (cur->mset->last == NULL) {
		cur->start = true;
		cur->end = false;
		return false;
	} 
	
	// Check if the cursor pointing to a node with the smallest element.
	else if (cur->now == cur->mset->first) {
		cur->start = true;
		cur->end = false;
		cur->now = NULL;
		return false;
	} 
	
	// Check the if the location of the cursor is at the end.
	else if (cur->end == true) {
		cur->now = cur->mset->last;
		cur->end = false;
		cur->start = false;
	} 
	
	// move the cursor to the previous element.
	else {
		cur->now = cur->now->prev;
		cur->end = false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////

