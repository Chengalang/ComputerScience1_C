#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

int processInputFile(char *filename)
{
	char buffer[1023], key[2];
	char *word, *str;
	int counter = 0;
	ListyString *mainList;

	FILE *ifp = NULL;

	if ((ifp = fopen(filename, "r")) == NULL)
		return 1;
	

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

			// printListyString(mainList);
			// printf("ListyLength: %d\n\n", mainList->length);

			free(str);
		}

		if (*word == '+')
		{
			fscanf(ifp, "%s", buffer);

			str = malloc(sizeof(char) * (strlen(buffer) + 1));

			strcpy(str, buffer);
			listyCat(mainList, str);

			// printListyString(mainList);
			// printf("ListyLength: %d\n\n", mainList->length);

			free(str);
		}

		if (*word == '-')
		{
			fscanf(ifp, "%s", key);

			str = malloc(sizeof(char));
			str = NULL;

			replaceChar(mainList, key[0], str);

			// printListyString(mainList);
			// printf("ListyLength: %d\n\n", mainList->length);

			free(str);
		}

		if (*word == '~')
		{
			reverseListyString(mainList);

			// printListyString(mainList);
			
		}

		if (*word == '?')
			printf("%d\n", mainList->length);
		

		if (*word == '!')
		{
			printListyString(mainList);

			// printf("ListyLength: %d\n\n", mainList->length);
		
		}

		counter++;
		free(word);
	}

	fclose(ifp);
	ifp = NULL;

	return 0;
}

ListyString *createListyString(char *str)
{
	int i, stringLength;

	ListyString *newList = malloc(sizeof(ListyString));
	ListyNode *temp;

	if (str == NULL || str == "")
	{
		newList->head = NULL;
		newList->length = 0;

		return newList;
	}

	stringLength = strlen(str);
	newList->length = 0;

	for (i = 0; i < stringLength; i++)
	{

		ListyNode *newNode = malloc(sizeof(ListyNode));

		if (i == 0)
		{
			newNode->data = str[i];
			newList->head = newNode;
			newList->length++;
			continue;
		}

		newNode->data = str[i];

		if (i == 1)
		{
			newList->head->next = newNode;
			newList->length++;
			temp = newNode;
			continue;
		}

		temp->next = newNode;
		temp = newNode;
		newList->length++;

		if (i == (stringLength - 1))
		{
			newNode->next = NULL;
			temp = NULL;
		}
	}

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
// to include listy->head == NULL parameter
ListyString *destroyListyString(ListyString *listy)
{
	if (listy == NULL || listy->head == NULL)
		return NULL;

	destroy_node(listy->head);

	free(listy);

	return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
	int i, listyLength = listy->length;

	if (listy == NULL)
		return NULL;
	if (listy->head == NULL)
	{
		ListyString *newList = malloc(sizeof(ListyString));
		newList->head = NULL;
		newList->length = 0;
	}

	ListyString *cloneList = malloc(sizeof(ListyString));
	ListyNode *temp;
	ListyNode *listyTemp;

	cloneList->length = 0;

	for (i = 0; i < listyLength; i++)
	{
		ListyNode *newNode = malloc(sizeof(ListyNode));

		if (i == 0)
		{
			newNode->data = listy->head->data;
			cloneList->head = newNode;
			cloneList->length++;
			continue;
		}

		if (i == 1)
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

		if (i == (listy->length - 1))
		{
			newNode->next = NULL;
			temp = NULL;
		}
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

	if (listy->length - 1 == 0)
	{
		listy->length = 0;
		free(listy);

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

		if (temp1->next == NULL && temp1->data == key)
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
			break;

		temp2 = temp1;
		temp1 = temp1->next;
	}
}

void replaceChar(ListyString *listy, char key, char *str)
{
	int i, stringLength, stringCount = 0, listyLength, keyFound = 0, keyCount = 0;
	ListyNode *temp1 = listy->head, *temp2;

	if (listy == NULL || listy->head == NULL)
		return;

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

		if ((i + 1) == listyLength && keyFound == 0)
			return;

		temp1 = temp1->next;
	}

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

			// if (i == (listyLength - 1))
			// 	temp2->next = temp1;

			continue;
		}

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

ListyNode *swapNodes(ListyNode *head, ListyString *listy, int listyLength)
{
	ListyNode *temp = NULL;

	if (head->next == NULL)
	{
		listy->head = head;
		return head;
	}

	temp = head;

	swapNodes(temp->next, listy, listy->length - 1);

	temp->next->next = temp;

	if (listyLength == listyLength)
		head->next = NULL;

	return 0;
}

void reverseListyString(ListyString *listy)
{
	int i;
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
		return;

	temp = listy->head;

	swapNodes(temp, listy, listy->length);
}

ListyString *listyCat(ListyString *listy, char *str)
{
	int i, stringLength;
	ListyNode *catTemp;

	stringLength = strlen(str);

	if (listy == NULL && str == NULL)
		return NULL;

	if (str == NULL || stringLength == 0)
		return listy;

	if (listy == NULL && stringLength != 0)
		return createListyString(str);

	if (listy == NULL && str != NULL && stringLength == 0)
		return createListyString(str);

	if (listy->length == 0 && stringLength != 0)
		return createListyString(str);

	catTemp = listy->head->next;

	for (i = 0; i < listy->length; i++)
	{
		if (catTemp->next == NULL)
			break;

		catTemp = catTemp->next;
	}

	for (i = 0; i < stringLength; i++)
	{
		if (str[i] == '\0')
			break; 

		ListyNode *catNode = malloc(sizeof(ListyNode));

		catNode->data = str[i];
		catTemp->next = catNode;
		catTemp = catNode;
		listy->length++;
	}

	return listy;
}

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
	if (listy == NULL || listy->head->data == '\0')
	{
		printf("(empty string)\n");
		return;
	}

	print_list(listy->head);
	printf("\n");
}	

int main(void)
{
	int i, length, success = 1;
	char *result = "Hewwo!";

	ListyNode *temp;
	ListyString *listy;
	ListyString *retval;

	length = strlen(result);

	// Create empty string.
	listy = malloc(sizeof(ListyString));
	listy->head = NULL;
	listy->length = 0;

	retval = listyCat(listy, "Hewwo!");

	if (listy != retval)
		success = 0;

	// Check contents of linked list.
	temp = listy->head;
	for (i = 0; i < length; i++)
	{
		if (temp->data != result[i])
			success = 0;
		temp = temp->next;
	}

	// Check for proper NULL termination of linked list.
	if (temp != NULL)
		success = 0;

	// Check length variable.
	if (listy->length != length)
		success = 0;

	printf("%s\n", success ? "Hooray!" : "fail whale :(");

	return 0;
}
