#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

void delete(ListyString *listy)
{

	free(listy->head);
	listy->head = NULL;
}

int main(void)
{
	
	ListyString *newList = malloc(sizeof(ListyString));
	ListyNode *newNode = malloc(sizeof(ListyNode));

	newList->head = newNode;
	newNode->data = 'c';
	newNode->next = NULL;

	delete(newList);

	if (newList->head == NULL)
		printf("Poof!\n");


	return 0;
}