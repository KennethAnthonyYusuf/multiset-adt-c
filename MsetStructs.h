#ifndef MSET_STRUCTS_H
#define MSET_STRUCTS_H

struct mset {
	struct node *tree;
	int size;
	int totalCount;
	struct node *first;
	struct node *last;
	MsetCursor cursor;
};

struct node {
	int elem;          
	int count;         
	struct node *left; 
	struct node *right;
	int height;
	int size;
	struct node *next;
	struct node *prev;
};

////////////////////////////////////////////////////////////////////////
// Cursors

struct cursor {
	Mset mset;
	struct node *now;
	bool start;
	bool end;
	bool validity;
};

////////////////////////////////////////////////////////////////////////

#endif

