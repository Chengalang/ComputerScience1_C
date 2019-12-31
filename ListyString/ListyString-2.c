// Kenny Cheng
// ke351939
// COP3502 - FALL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

// Takes filename from argv[1] and
// processes the command args.
int processInputFile(char *filename)
{
	char buffer[1023], key[2];
	char *word, *str;
	int counter = 0;
	ListyString *mainList;

	FILE *ifp = NULL;

	if ((ifp = fopen(filename, "r")) == NULL)
		return 1;
	
	// Scans the txt file until file has ended
	// Taken from string-buffer-basic.c notes
	// and ideas form Glowworm.c
	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		word = malloc(sizeof(char) * (strlen(buffer) + 1));

		strcpy(word, buffer);

		if (counter == 0)
			mainList = createListyString(word);

		if (*word == '@')
		{
			fscanf(ifp, "%s", key);
			fscanf(ifp, "%s", buffer);
			
			str = malloc(sizeof(char) * (strlen(buffer) + 1));

			strcpy(str, buffer);
			replaceChar(mainList, key[0], str);
			free(str);
		}

		if (*word == '+')
		{
			fscanf(ifp, "%s", buffer);

			str = malloc(sizeof(char) * (strlen(buffer) + 1));

			strcpy(str, buffer);
			listyCat(mainList, str);
			free(str);
		}

		if (*word == '-')
		{
			fscanf(ifp, "%s", key);

			str = malloc(sizeof(char));
			str = NULL;

			replaceChar(mainList, key[0], str);
			free(str);
		}

		if (*word == '~')
			reverseListyString(mainList);

		if (*word == '?')
			printf("%d\n", mainList->length);
		

		if (*word == '!')
			printListyString(mainList);

		counter++;
		free(word);
	}

	// Making sure we practice good
	// cleaning habits.
	destroyListyString(mainList);
	fclose(ifp);
	ifp = NULL;

	return 0;
}

// Created this function when listyCat() tries to insert
// into an empty linked list still allocated.
void node_insert(ListyString *listy, char *str)
{	
	int i, stringLength;
	ListyNode *temp, *newNode;

	stringLength = strlen(str);

	if (listy->length > 1)
	{
		temp = listy->head;

		for (i = 0; i < listy->length; i++)
		{
			if (temp->next == NULL)
				continue;

			temp = temp->next;	
		}

		for (i = 0; i < stringLength; i++)
		{
			newNode = malloc(sizeof(ListyNode));

			newNode->data = str[i];
			temp->next = newNode;
			temp = newNode;
			listy->length++;
		}

		// This will haunt me 
		// in my sleep forever.
		temp->next = NULL;
	}

	if (listy->length <= 1)
	{
		for (i = 0; i < stringLength; i++)
		{
			ListyNode *newNode = malloc(sizeof(ListyNode));

			if (listy->length == 0)
			{
				newNode->data = str[i];
				listy->head = newNode;
				listy->length++;
				continue;
			}

			newNode->data = str[i];

			if (listy->length == 1)
			{
				listy->head->next = newNode;
				listy->length++;
				temp = newNode;
				continue;
			}

			temp->next = newNode;
			temp = newNode;
			listy->length++;
			newNode->next = NULL;
		}
	}
}

// Creates a new ListyString with
// *str characters.
ListyString *createListyString(char *str)
{
	int i, stringLength;

	// Using calloc() to avoid setting newList->length to 0.
	ListyString *newList = calloc(1, sizeof(ListyString));

	if (str == NULL || str[0] == '\0')
		return newList;

	node_insert(newList, str);

	return newList;
}

// Taken from fancy-linked-list.c
ListyNode *destroy_node(ListyNode *node)
{
	if (node == NULL)
		return NULL;

	destroy_node(node->next);
	free(node);

	return NULL;
}

// Taken from fancy-linked-list.c and slightly modified
// to include listy->head == NULL parameter.
ListyString *destroyListyString(ListyString *listy)
{
	if (listy == NULL || listy->head == NULL)
		return NULL;

	destroy_node(listy->head);
	free(listy);

	return NULL;
}

// Takes the original Listy and clones it by using 
// the same data but not the same addresses .
ListyString *cloneListyString(ListyString *listy)
{
	int i, listyLength = listy->length;
	ListyNode *temp;
	ListyNode *listyTemp;

	if (listy == NULL)
		return NULL;

	// Base case of replicating an allocated Listy
	// that has no nodes .
	if (listy->head == NULL)
	{
		ListyString *newList = calloc(1, sizeof(ListyString));
		return newList;
	}

	// Using calloc() more often after my clean-up phase
	// of the code. Reduced a lot of lines for me.
	ListyString *cloneList = calloc(1, sizeof(ListyString));

	for (i = 0; i < listyLength; i++)
	{
		ListyNode *newNode = malloc(sizeof(ListyNode));

		if (cloneList->length == 0)
		{
			newNode->data = listy->head->data;
			cloneList->head = newNode;
			cloneList->length++;
			continue;
		}

		if (cloneList->length == 1)
		{
			newNode->data = listy->head->next->data;
			cloneList->head->next = newNode;
			listyTemp = listy->head->next->next;
			temp = newNode;
			cloneList->length++;
			continue;
		}

		newNode->data = listyTemp->data;
		temp->next = newNode;
		temp = newNode;
		listyTemp = listyTemp->next;
		cloneList->length++;
		newNode->next = NULL;
	}

	return cloneList;	
}

// Made this function to delete key if str passed was
// NULL or empty to avoid replaceChar() from getting
// too big and hard to read.
void deleteKey(ListyString *listy, char key)
{
	int i, listyLength = listy->length;

	ListyNode *temp1 = listy->head, *temp2;

	if (listy->length == 1)
	{
		free(listy->head);
		listy->length = 0;
		return;
	}

	for (i = 0; i < listyLength; i++)
	{
		if (listy->head->data == key)
		{
			listy->head = temp1->next;
			free(temp1);
			listy->length--;
			temp1 = listy->head;
			continue;
		}

		if (temp1->data == key && temp1->next == NULL)
		{
			free(temp1);
			listy->length--;
			temp2->next = NULL;
			continue;
		}

		if (temp1->data == key)
		{
			temp2->next = temp1->next;
			free(temp1);
			listy->length--;
			temp1 = temp2->next;
			continue;
		}

		if (temp1->next == NULL)
			continue;

		temp2 = temp1;
		temp1 = temp1->next;
	}

	if (listy->length == 0)
		free (listy->head);
}

// Searches listy to find the proper 'key' and replaces
// it with str while keep listy connected.
void replaceChar(ListyString *listy, char key, char *str)
{
	int i, stringLength, stringCount = 0, listyLength, keyFound = 0, keyCount = 0;
	ListyNode *temp1 = listy->head, *temp2;

	if (listy == NULL || listy->head == NULL)
		return;

	// Calling to delete key if str is empty.
	if (str == NULL || str[0] == '\0')
	{
		deleteKey(listy, key);
		return;
	}

	listyLength = listy->length;
	stringLength = strlen(str);

	for (i = 0; i < listyLength; i++)
	{
		if (temp1->data == key)
			keyFound++;

		// End function early if we loop through list
		// and key was never found.
		if ((i + 1) == listyLength && keyFound == 0)
			return;

		temp1 = temp1->next;
	}

	// Using the lengths and how many times key was found to determine
	// the new length of listy after chars have been replaced.
	listyLength = listy->length + ((stringLength * keyFound) - keyFound);
	temp1 = listy->head;

	for (i = 0; i < listyLength; i++)
	{
		if (i == 0 && temp1->data == key)
		{
			ListyNode *newNode = malloc(sizeof(ListyNode));
			newNode->data = str[stringCount++];
			newNode->next = temp1->next;
			free(temp1);

			if (listy->length == 1)
			{
				listy->head = newNode;
				newNode->next = NULL;
				temp2 = newNode;
				temp1 = NULL;
				continue;
			}

			listy->head = newNode;
			temp1 = newNode->next;
			temp2 = newNode;
			continue;
		}

		if (stringCount != stringLength && stringCount != 0)
		{
			ListyNode *newNode = malloc(sizeof(ListyNode));
			newNode->data = str[stringCount++];
			newNode->next = temp1;
			temp2->next = newNode;
			temp2 = newNode;

			continue;
		}

		// Reset string index once we have 
		// replaced previous key.
		if (stringCount == stringLength)
			stringCount = 0;

		if (temp1->data == key)
		{
			ListyNode *newNode = malloc(sizeof(ListyNode));
			newNode->data = str[stringCount++];
			newNode->next = temp1->next;
			free(temp1);
			temp2->next = newNode;
			temp1 = newNode->next;
			temp2 = newNode;
			continue;
		}

		temp2 = temp1;
		temp1 = temp1->next;
	}

	listy->length = listyLength;
}

// Auxiliary function with recursive call to reverse listy.
void swapNodes(ListyNode *head, ListyString *listy, int listyLength)
{
	ListyNode *temp = NULL;

	if (head->next == NULL)
	{
		listy->head = head;
		return;
	}

	temp = head;

	swapNodes(temp->next, listy, listy->length - 1);

	temp->next->next = temp;

	if (listyLength == listyLength)
		head->next = NULL;
}

// Takes a ListyString and reverses the chars.
void reverseListyString(ListyString *listy)
{
	int i;
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
		return;

	temp = listy->head;

	swapNodes(temp, listy, listy->length);
}

// Takes the str and adds it to the end of current
// ListyString with some implementations if ListyString
// is empty or NULL.L
ListyString *listyCat(ListyString *listy, char *str)
{
	int i, stringLength, listyLength;
	ListyNode *catTemp;

	if (str != NULL)
		stringLength = strlen(str);

	if (listy == NULL && str == NULL)
		return NULL;

	if (str == NULL || stringLength == 0)
		return listy;

	if (listy->length == 0 && stringLength > 0)
	{
		node_insert(listy, str);
		return listy;
	}

	if (listy == NULL && stringLength > 0)
		return createListyString(str);

	if (listy == NULL && str != NULL && stringLength == 0)
		return createListyString(str);

	// Standard listyCat if 
	// listy already has a string
	// and str isn't NULL or empty.
	node_insert(listy, str);

	return listy;
}

// Used to compare too ListyStrings and usefull
// to check cloneListyString().
int listyCmp(ListyString *listy1, ListyString *listy2)
{
	int i;
	ListyNode *temp1, *temp2;

	if (listy1 == NULL && listy2 == NULL)
		return 0;

	if (listy1 == NULL && listy2->length == 0)
		return -1;
	
	if (listy2 == NULL && listy1->length == 0)
		return -1;

	if (listy1->length == 0 && listy2->length == 0)
		return 0;

	if (listy1->length != listy2->length)
		return -1;

	temp1 = listy1->head;
	temp2 = listy2->head;

	for (i = 0; i < listy1->length; i++)
	{
		if (temp1->data != temp2->data)
			return -1;

		if (temp1->next == NULL || temp2->next == NULL)
			break;

		temp1 = temp1->next;
		temp2 = temp2->next;
	}

	return 0;
}

// Returns the current length value of Listy
// by calling the listy->length int.
int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	if (listy != NULL && listy->head == NULL)
		return 0;

	return listy->length;
}

// Taken from fancy-linked-list.c
void print_list(ListyNode *head)
{
	if (head == NULL)
		return;

	printf("%c", head->data);
	print_list(head->next);
}

// Taken from fancy-linked-list.c 
void printListyString(ListyString *listy)
{
	if (listy == NULL || listy->length == 0)
	{
		printf("(empty string)\n");
		return;
	}

	print_list(listy->head);
	printf("\n");
}	

double difficultyRating(void)
{
	return 3.5;
}

double hoursSpent(void)
{
	return 25.0;
}

int main(int argc, char **argv)
{
	processInputFile(argv[1]);

	return 0;
}
