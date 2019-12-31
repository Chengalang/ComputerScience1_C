// Kenny Cheng
// ke351939
// COP3502 - FALL

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

// Creates LonelyPartyArray by allocating space and initializing all cells.
// Performs NULL check after malloc() call.
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
	int capacity, i;
	LPA *party = malloc(sizeof(LonelyPartyArray));

	if (party == NULL)
		return NULL;

	if ( (num_fragments <= 0) || (fragment_length <= 0) )
		return NULL;

	capacity = num_fragments * fragment_length;
	party->num_fragments = num_fragments;
	party->fragment_length = fragment_length; 
	party->num_active_fragments = 0;
	party->size = 0;

	party->fragments = malloc(sizeof(int *) * num_fragments);
	if (party->fragments == NULL)
	{
		free(party);
		return NULL;
	}

	party->fragment_sizes = malloc(sizeof(int) * num_fragments);
	if (party->fragment_sizes == NULL)
	{
		free(party->fragments);
		free(party);
		return NULL;
	}

	for (i = 0; i < num_fragments; i++)
		party->fragments[i] = NULL;

	for (i = 0; i < num_fragments; i++)
		party->fragment_sizes[i] = 0;

	printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", 
		capacity, party->num_fragments);

	return party;
}

// Destroys passed in party array and frees all memory.
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
	int i;

	if (party == NULL)
		return NULL;
	
	// Free individual cells first to ensure no memory leaks.
	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
		{
			free(party->fragments[i]);
		}
	}	

	// Party is freed last to avoid any leaks.
	free(party->fragment_sizes);
	free(party->fragments);
	free(party);

	printf("-> The LonelyPartyArray has returned to the void.\n");

	return NULL;
}

// Makes a copy of passed in party but does not use any of its 
// addresses, only the data inputs.
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
	int capacity, i, j;
	LPA *partyTwo = NULL;

	if (party == NULL)
		return NULL;

	if ((party->num_fragments <= 0) || (party->fragment_length <= 0))
		return NULL;

	partyTwo = malloc(sizeof(LonelyPartyArray));
	if (partyTwo == NULL)
		return NULL;

	partyTwo->num_fragments = party->num_fragments;
	partyTwo->fragment_length = party->fragment_length; 
	partyTwo->num_active_fragments = 0;
	partyTwo->size = 0;
	capacity = partyTwo->num_fragments * partyTwo->fragment_length;

	// We make sure if malloc() fails that we free the memory of any 
	// previous successful malloc() calls.
	partyTwo->fragments = malloc(sizeof(int *) * partyTwo->num_fragments);
	if (partyTwo->fragments == NULL)
	{
		free(partyTwo);
		return NULL;
	}

	partyTwo->fragment_sizes = malloc(sizeof(int) * partyTwo->num_fragments);
	if (partyTwo->fragment_sizes == NULL)
	{
		free(partyTwo->fragments);
		free(partyTwo);
		return NULL;
	}

	// Loops through the number of fragments in array being copied
	// and collects the data if cells are marked UNUSED or non NULL
	for (i = 0; i < partyTwo->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
		{
			partyTwo->fragments[i] = malloc(sizeof(int) * partyTwo->fragment_length);
			if (partyTwo->fragments[i] == NULL)
			{
				free(partyTwo->fragment_sizes);
				free(partyTwo->fragments);
				free(partyTwo);
				return NULL;
			}

			partyTwo->num_active_fragments++;

			for (j = 0; j < partyTwo->fragment_length; j++)
				partyTwo->fragments[i][j] = UNUSED;
		}

		if (partyTwo->fragments[i] != NULL)
		{
			for (j = 0; j < partyTwo->fragment_length; j++)
			{
				if (party->fragments[i] != NULL && party->fragments[i][j] != UNUSED)
				{
					partyTwo->fragments[i][j] = party->fragments[i][j];
					partyTwo->fragment_sizes[i]++;
					partyTwo->size++;
				}
			}
		}
	}

	capacity = (partyTwo->num_fragments * partyTwo->fragment_length);

	printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n", 
		capacity, partyTwo->num_fragments);

	return partyTwo;
}

// Used to insert key parameter into designated index.
int set(LonelyPartyArray *party, int index, int key)
{
	int i, j, indexLoc, offset;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in set().\n");

		return LPA_FAILURE;
	}

	// As int variables round to the floor, we can use basic
	// math to locate which index to put the key.
	indexLoc = (index / party->fragment_length);
	offset = (index % party->fragment_length);

	if (party != NULL && (index < 0) || 
		(index > ((party->num_fragments * party->fragment_length) - 1)) )
	{
		printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n",
			index, indexLoc, offset);

		return LPA_FAILURE;
	} 

	// If the index has not been created, we will allocate the space.
	if (party->fragments[indexLoc] == NULL)
	{
		party->fragments[indexLoc] = malloc(sizeof(int) * party->fragment_length);
		if (party->fragments[indexLoc] == NULL)
			return LPA_FAILURE;

		printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", 
			indexLoc, party->fragment_length, (party->fragment_length) * indexLoc, 
			((party->fragment_length * (indexLoc + 1)) - 1));

		for (j = 0; j < party->fragment_length; j++)
			party->fragments[indexLoc][j] = UNUSED;	

		party->fragments[indexLoc][offset] = key;
		party->fragment_sizes[indexLoc]++;
		party->num_active_fragments++;		
		party->size++;

		return LPA_SUCCESS;
	}		
	else if (party->fragments[indexLoc][offset] == UNUSED)
	{
		party->fragments[indexLoc][offset] = key;
		party->fragment_sizes[indexLoc]++;
		party->size++;
		
		return LPA_SUCCESS;
	}
	else
		party->fragments[indexLoc][offset] = key;
		return LPA_SUCCESS;
}

// Used to look up a specific data the same way set() functions.
int get(LonelyPartyArray *party, int index)
{
	int offset, indexLoc;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");

		return LPA_FAILURE;
	}

	indexLoc = (index / party->fragment_length);
	offset = (index % party->fragment_length);

	// Determines if the index is out of bounds and prompts an
	// invalid index alert.
	if (party != NULL && (index < 0) || 
		(index > ((party->num_fragments * party->fragment_length) - 1)))
	{
		printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n",
			index, indexLoc, offset);

		return LPA_FAILURE;
	} 
	
	// Return value of function determined by UNUSED or INT variable.
	if ((index >= 0) && (index <= ((party->num_fragments * party->fragment_length) - 1)) 
		&& party->fragments[indexLoc] == NULL)
	{
		return UNUSED;
	} 
	else if ((index >= 0) && (index < ((party->num_fragments * party->fragment_length) - 1)) 
		&& party->fragments[indexLoc] != NULL)
	{
		return party->fragments[indexLoc][offset];
	}
}

// Used to remove a key and set array cell to UNUSED
// and always deallocates the memory.
int delete(LonelyPartyArray *party, int index)
{
	int offset, indexLoc;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in delete().\n");

		return LPA_FAILURE;
	}

	offset = (index % party->fragment_length);
	indexLoc = (index / party->fragment_length);

	if (party != NULL && (index < 0) || 
		(index > ((party->num_fragments * party->fragment_length) - 1)))
	{
		printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n",
			index, indexLoc, offset);

		return LPA_FAILURE;
	} 

	if (party->fragments[indexLoc] == NULL || party->fragments[indexLoc][offset] == UNUSED)
		return LPA_FAILURE;

	// Checks to see if the index being access for delete() has an int
	// then proceeds to delete if so with deallocating process if all
	// cells have been set to UNUSED.
	if (party->fragments[indexLoc] != NULL && party->fragments[indexLoc][offset] != UNUSED)
	{
		party->fragments[indexLoc][offset] = UNUSED;
		party->size--;
		party->fragment_sizes[indexLoc]--;

		if (party->fragment_sizes[indexLoc] == 0)
		{
			free(party->fragments[indexLoc]);
			party->fragments[indexLoc] = NULL;
			party->num_active_fragments--;
			printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n",
				indexLoc, party->fragment_length, (party->fragment_length) * indexLoc, 
				((party->fragment_length * (indexLoc + 1)) - 1));
		}

		return LPA_SUCCESS;
	}

	// If the party array index passed in has no values then it will be deallocated
	// and freed of memory.
	if (party->fragments[indexLoc] != NULL && party->fragment_sizes[indexLoc] == 0)
	{
		free(party->fragments[indexLoc]);
		party->fragments[indexLoc] = NULL;
		party->num_active_fragments--;
		printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d\n",
			indexLoc, party->fragment_length, (party->fragment_length) * indexLoc, 
			((party->fragment_length * (indexLoc + 1)) - 1));

		return LPA_SUCCESS;
	}
}

// Uses key parameter then loops through the whole LPA
// and tries to locate specific key
int containsKey(LonelyPartyArray *party, int key)
{
	int i, j;
	
	if (party == NULL)
		return 0;
	
	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
		{
			for (j = 0; j < party->fragment_length; j++)
			{
				if (party->fragments[i][j] == key)
					return 1;
			}
		}
	}

	return 0;
}

// Uses index parameter to determine if fragment has been created
// with specific index
int isSet(LonelyPartyArray *party, int index)
{
	int i, j, indexLoc, offset;

	if (party == NULL)
	{
		return 0;
	}

	indexLoc = (index / party->fragment_length);
	offset = (index % party->fragment_length);

	if (party->fragments[indexLoc] == NULL
		|| party->fragments[indexLoc][offset] == UNUSED)
	{
		return 0;
	}

	if (party != NULL && (index < 0) || 
		(index > ((party->num_fragments * party->fragment_length) - 1)) )
	{
		return 0;
	} 

	if (party->fragments[indexLoc][offset] != UNUSED)
	{
		return 1;
	}
}

// Simply prints the int value if index parameter is found
int printIfValid(LonelyPartyArray *party, int index)
{
	int indexLoc, offset;

	if (party == NULL)
		return LPA_FAILURE;

	indexLoc = (index / party->fragment_length);
	offset = (index % party->fragment_length);

	if (party != NULL && (index < 0) || 
		(index > ((party->num_fragments * party->fragment_length) - 1)) )
		return LPA_FAILURE;

	if (party->fragments[indexLoc] == NULL || party->fragments[indexLoc][offset] == UNUSED)
		return LPA_FAILURE;

	printf("%d\n", party->fragments[indexLoc][offset]);

	return LPA_SUCCESS;
}

// Resets the entire LPA to right after it was created.
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
	int i, j;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
		return party;
	}

	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
		{
			for (j = 0; j < party->fragment_length; j++)
			{
				if (party->fragments[i][j] != UNUSED)
				{
					party->fragments[i][j] = UNUSED;
				}
			}

			if (party->fragment_sizes[i] != 0)
			{
				party->fragment_sizes[i] = 0;
			}

			// After deallocation, I set to NULL 
			// for safe measures.
			if (party->fragments[i] != NULL)
			{
				free(party->fragments[i]); 
				party->fragments[i] = NULL;
			}
		}
	}

	party->size = 0;
	party->num_active_fragments = 0;

	printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n",
		party->num_fragments * party->fragment_length, party->num_fragments);

	return party;
}

// Returns the current size of the LPA.
int getSize(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;

	return (party->size);
}

// Returns the maximum size of ints in the LPA.
int getCapacity(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;
	
	return (party->num_fragments * party->fragment_length);
}

// Returns the maximum number of ints the LPA can hold
// from allocated malloc() calls.
int getAllocatedCellCount(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;

	return (party->num_active_fragments * party->fragment_length);
}

// Returns the value of the array size in bytes if memory 
// was not managed.
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
	long long unsigned int byteStorage;

	if (party == NULL)
		return 0;
	
	byteStorage = (party->num_fragments * party->fragment_length * sizeof(int));

	return byteStorage;
}

// Returns the value of the array size in bytes of with memory
// management being utilized. This is used to compare how important
// memory management can be.
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
	long long unsigned int byteStorage;
	
	if (party == NULL)
		return 0;

	byteStorage = (sizeof(LPA*) + sizeof(LPA) 
		+ (sizeof(int *) * party->num_fragments) 
		+ (sizeof(int) * party->num_fragments) 
		+ (sizeof(int) * party->num_active_fragments * party->fragment_length));

	return byteStorage;
}

// Returns a double variable for the difficulty level.
double difficultyRating(void)
{
	return 2.0;
}

// Returns a double variable for the amount of time spent.
double hoursSpent(void)
{
	return 10.0;
}
