// Kenny Cheng
// ke351939
// COP3502 - FALL

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

// Used for Stacks traversal
// in KindredSpirits().
typedef struct StackNode
{
	struct StackNode *below;
	int data;
} StackNode;

typedef struct Stack
{
	StackNode *top;
	int size;
} Stack;

// Standard createNode() function
// that takes data and returns allocated
// memory for a new node.
node *createNode(int data)
{
	node *newNode = calloc(1, sizeof(node));

	newNode->data = data;

	return newNode;
}


// Both functions used as a 
// base case in KindredSpirits()
// on the chance that both trees 
// are not equal.
int countNodes(node *a)
{
	if (a == NULL)
		return 0;

	return 1 + countNodes(a->left) + countNodes(a->right);
}

int compareSize(node *a, node *b)
{
	int aSize, bSize;

	aSize = countNodes(a);
	bSize = countNodes(b);

	if (aSize != bSize)
		return 0;

	return 1;
}

// Takes the root node of two trees and
// returns 1 or 0 if the trees are reflections
// of each other.
int isReflection(node *a, node *b)
{
	// I didn't add this to compareSize()
	// because both could be NULL and compareSize()
	// wouldn't end this function early which
	// could cause a segfault.
	if (a == NULL && b == NULL)
		return 1;
	else if (compareSize(a, b) == 0)
		return 0;

	if (a->data != b->data)
		return 0;

	// Traverses down the height of both trees twice with && operator so 
	// anything beside T && T will return F which means they are not reflections
	return (isReflection(a->left, b->right) && isReflection(a->right, b->left));	
}

node *reflectionInsert(node *original, node *copyCat)
{
	if (original == NULL)
		return NULL;

	copyCat = createNode(original->data);

	copyCat->right = reflectionInsert(original->left, copyCat->right);
	copyCat->left = reflectionInsert(original->right, copyCat->left);

	return copyCat;
}

// Takes root of tree to be copied.
// Aux functions to allocate mem
// for new reflection tree. 
// Does not alter original tree.
node *makeReflection(node *root)
{
	if (root == NULL)
		return NULL;

	node *copyCatTree = calloc(1, sizeof(node));

	copyCatTree->data = root->data;
	copyCatTree->right = reflectionInsert(root->left, copyCatTree->right);
	copyCatTree->left = reflectionInsert(root->right, copyCatTree->left);

	return copyCatTree;
}

// Helper function to clean up
// some of the push() so the first 
// element we put into the stack 
// doesn't segfault our program.
int isEmpty(Stack *s)
{
	return (s == NULL || s->size == 0);
}

// push() and pop() stacks to help with traversing
// within kindredSpirits().
void push(Stack *s, int data)
{
	StackNode *newNode = calloc(1, sizeof(StackNode));

	if (isEmpty(s))
	{
		newNode->data = data;
		s->top = newNode;
		s->size++;
	}
	else
	{
		newNode->data = data;
		newNode->below = s->top;
		s->top = newNode;
		s->size++;
	}
}

int pop(Stack *s)
{
	StackNode *tempNode;
	int value;

	tempNode = s->top;
	s->top = tempNode->below;
	value = tempNode->data;
	free(tempNode);
	s->size--;

	return value;
}

// preorder and postorder traversal functions 
// that return a stack that is used to compare
// if two trees are KindredSpirits.
// Recursive helper functions to traverse down
// the trees. 
void preorderRecursive(Stack *s, node *root)
{
	if (root == NULL)
		return;

	push(s, root->data);
	preorderRecursive(s, root->left);
	preorderRecursive(s, root->right);
}

void postorderRecursive(Stack *s, node *root)
{
	if (root == NULL)
		return;

	postorderRecursive(s, root->left);
	postorderRecursive(s, root->right);
	push(s, root->data);
}

Stack *preorderStack(node *root)
{
	if (root == NULL)
		return NULL;
	
	// Gets freed within compareStacks().
	Stack *preStack = calloc(1, sizeof(Stack));

	preorderRecursive(preStack, root);

	return preStack;
}

Stack *postorderStack(node *root)
{
	if (root == NULL)
		return NULL;

	// Gets freed within compareStacks().
	Stack *postStack = calloc(1, sizeof(Stack));

	postorderRecursive(postStack, root);

	return postStack;
}

// Helper function that takes two root nodes
// and compares if the functions are KindredSpirits.
// Function works as the 'gut' of kindredSpirits()
// to keep that function cleaner and as a 'guide'.
int compareStacks(node *a, node *b)
{
	Stack *tempPost, *tempPre;
	int i, nodeCount = countNodes(a);
	int aValue, bValue, passFail = 1;

	tempPre = preorderStack(a);
	tempPost = postorderStack(b);

	// Pops the values in the order they
	// were pushed. If values do not
	// equal, then the two trees
	// are not KindredSpirits.
	for (i = 0; i < nodeCount; i++)
	{
		aValue = pop(tempPre);
		bValue = pop(tempPost);

		// passFail value instead of
		// return 0 to avoid mem leaks.
		if (aValue != bValue)
			passFail = 0;
	}

	// Clean up after yourself.
	free(tempPre);
	free(tempPost);

	return passFail;
}

// Used more of a 'guide' or reference while
// other functions do the main work. 
// Helps keep things more organized if modifications
// need to be done.
int kindredSpirits(node *a, node *b)
{
	if (a == NULL && b == NULL)
		return 1;
	else if (compareSize(a, b) == 0)
		return 0;

	// Allows both trees to be traversed in preorder than
	// compared to postorder of opposite tree. 
	// The || operator returns function as true only 
	// when one comparison is true.
	return (compareStacks(a, b) || compareStacks(b, a));
}

double difficultyRating(void)
{
	return 2.5;
}

double hoursSpent(void)
{
	return 3.5;
}
