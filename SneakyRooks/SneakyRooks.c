// Kenny Cheng
// ke351939
// COP3502 - FALL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SneakyRooks.h"

// Function that takes 3 parameters and outputs 1 (safe) or 0 (unsafe)
// based on the string array being passed. 
int allTheRooksAreSafe(char **rookStrings, int numRooks, int boardSize)
{
	int i, j, length;
	int rowDigit = 0, colDigit = 0;

	// Base case to save time.
	if (numRooks == 0)
		return 1;

	// Allocate memory for array to hold integers from [1, boardsize]
	// added the + 1 to accommodate for the board not starting at 0. 
	int *rowSafetyCheck = calloc(boardSize + 1, sizeof(int));
	int *colSafetyCheck = calloc(boardSize + 1, sizeof(int));

	// Loop through each rook string to get
	// the position of each Rook.
	for (i = 0; i < numRooks; i++)
	{
		// O(k) operation to determine string length.
		length = strlen(rookStrings[i]);

		if (length == 0)
			break;

		// O(k) operation that grabs the location of each Rook using
		// base conversion to determine index.
		// 'Numbers' determine row and 'letters' determine column.
		for (j = 0; j < length; j++)
		{
			if (rookStrings[i][j] >= 'a' && rookStrings[i][j] <= 'z')
			{
				colDigit *= 26;
				colDigit += (rookStrings[i][j] - 'a' + 1);
			}
			else if (rookStrings[i][j] >= '0' && rookStrings[i][j] <= '9')
			{
				rowDigit *= 10;
				rowDigit += (rookStrings[i][j] - '0');
			}
		}
		
		// Used the basic idea behind bucketsort to keep an array of ints
		// and counter based on index. Since Rooks can go left, right, up,
		// or down, if a count becomes more than 1, not all Rooks are safe. 
		if (++rowSafetyCheck[rowDigit] > 1 || ++colSafetyCheck[colDigit] > 1)
		{	
			// Clean up after yourself.
			free(rowSafetyCheck);
			free(colSafetyCheck);
			return 0;
		}

		// Reset index.
		rowDigit = 0;
		colDigit = 0;
	}

	// Clean up after yourself.
	free(rowSafetyCheck);
	free(colSafetyCheck);

	return 1;
}

// Takes a string that determines the position of Rook and uses a 
// rookCoordinate struct to store the data. 
void parseCoordinateString(char *rookString, Coordinate *rookCoordinate)
{
	int i, length = strlen(rookString);

	// Using baseconversion to determine where each
	// string of 'locations' will end up.
	for (i = 0; i < length; i++)
	{
		if (rookString[i] >= '0' && rookString[i] <= '9')
		{	
			rookCoordinate->row *= 10;
			rookCoordinate->row += (rookString[i] - '0');
		}
		else if (rookString[i] >= 'a' && rookString[i] <= 'z')
		{
			rookCoordinate->col *= 26;
			rookCoordinate->col += (rookString[i] - 'a' + 1);
		}
	}
}

double difficultyRating(void)
{
	return 2.0;
}

double hoursSpent(void)
{
	return 2.5;
}