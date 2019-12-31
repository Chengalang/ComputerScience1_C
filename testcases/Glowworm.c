// Kenny Cheng
// ke351939

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Glowworm.h"

// Returns a double variable for the difficulty level.
double difficultyRating(void)
{
	return 2.5;
}

// Returns a double variable for the amount of time spent.
double hoursSpent(void)
{
	return 8.0;
}

// Althought not required, I used this to organize a little bit.
void welcomeMessage(int maxLength, char *argv)
{
	int i;

	printf("%s\n\n", argv);
	printf("Glowworm appears! Hooray!\n");
	printf("~OG\n");
			
	for(i = 0; i < maxLength; i++)
	{
		printf("=");
	}

	printf("\n\n");
}

// Reads parameters from main() and prints the worm
void printGlowworm(int tail, int head, int maxLength, int death)
{
	int i, j;
	int mainSegLoc = (head - 1), counter = 0;

	for(i = 0; i < maxLength; i++)
	{

		// counter is used for spacing parameters.
		if(tail == i)
		{
			printf("~");
			counter++;
		}

		// For wrapping and weird growing spurts.
		if(head == 0)
		{
			mainSegLoc = (maxLength - 1);
		}

		// 'else if' statements fails when head == 0
		// Using death counter to determine when to print then stop program. SAD.
		if(mainSegLoc == i)
		{
			if(death == 1)
			{
				printf("X");
			}
			else
			{
				printf("O");
			}
		}

		// Print head of the worm based on position and death counter. 
		if(head == i)
		{	
			if(death == 1)
			{
				printf("x");
				counter++;
			}
			else
			{
				printf("G");
				counter++;
			}
		}

		// Parameters for when to print the segments of the worm.
		// This is where I used the counter to stop any additional spaces after the worm has
		// finished printing. I also could have used else if(...&& ((head > tail) && (i > head)) ) 
		// instead of the counter, but decided on it for more control of variables.
		// The additonal space at the end and beggining of the parameters ( ) is for readible purposes.
		if( (i > tail) && (i < (head - 1)) && (tail < head) )
		{
			printf("o");
		}
		else if( (i > tail) && (tail > head) && ((i < (maxLength-1)) || (i != mainSegLoc)) )
		{
			printf("o");
		}
		else if( (tail > head) && (mainSegLoc > 0) && (i < mainSegLoc) && (mainSegLoc < head) )
		{
			printf("o");
		}
		else if( (i != head) && (i != tail) && (i != mainSegLoc) && ((tail > head) || (tail > 0)) && (counter != 2) )
		{
			printf(" ");
		}	
	}

	printf("\n");

	// Prints the ledge based on maxLength.
	for(i = 0; i < maxLength; i++)
	{
		printf("=");
	}

	printf("\n\n");

}

// Where I implemented simple math, for loops, and if else statements
// to determine the positioning for printGlowworm() to work.
int main(int argc, char **argv)
{

	// wormLength counter initiliazed at 3 to keep track for chilling purposes.
	// tail and head initialized at starting positions.
	int i, j;
	int tail = 0, head = 2, maxLength = 0, stringLength = 0, death = 0, wormLength = 3;

	// for loop to read each char in the 2d array to determine 
	// the action of the worm.
	for(i = 0; i < argc; i++)
	{

		if(i == 1)
		{
			maxLength = atoi(argv[i]);
		}
		else if(i == 2)
		{
		
			welcomeMessage(maxLength, argv[i]);
			stringLength = strlen(argv[i]);

			// Nested for loop to cycle through all the different char
			// within argv[2][j].
			for(j = 0; j < stringLength; j++)
			{

				// When char 'o', 'O', or '@' is read the worm GROWS
				if( (argv[i][j] == 'o') || (argv[i][j] == 'O') || (argv[i][j] == '@') )
				{
					if( (wormLength >= (maxLength)) )
					{
						printf("Glowworm chills:\n");
						printGlowworm(tail, head, maxLength, death);
					}
					else if(head == (maxLength - 1))
					{
						printf("Glowworm grows:\n");
						head = 0;
						wormLength++;
						printGlowworm(tail, head, maxLength, death);
					}
					else
					{
						printf("Glowworm grows:\n");
						head++;
						wormLength++;
						printGlowworm(tail, head, maxLength, death);
					}
				}

				// When char 's' or 'S' is read the worm SHRINKS
				else if( (argv[i][j] == 's') || (argv[i][j] == 'S') )
				{
					if(wormLength == 3)
					{
						printf("Glowworm chills:\n");
						printGlowworm(tail, head, maxLength, death);
					}
					else if(head == 0)
					{
						printf("Glowworm shrinks:\n");
						head = (maxLength - 1);
						wormLength--;
						printGlowworm(tail, head, maxLength, death);
					}
					else
					{
						printf("Glowworm shrinks:\n");
						head--;
						wormLength--;
						printGlowworm(tail, head, maxLength, death);
					}
				}

				// When char '-' or '=' is read the worm INCHES FORWARD
				else if( (argv[i][j] == '-') || (argv[i][j] == '=') )
				{
					if(head == (maxLength - 1))
					{
						printf("Glowworm inches forward:\n");
						head = 0;
						tail++;
						printGlowworm(tail, head, maxLength, death);
					}
					else if(tail == (maxLength - 1))
					{
						printf("Glowworm inches forward:\n");
						tail = 0;
						head++;
						printGlowworm(tail, head, maxLength, death);
					}
					else
					{
						printf("Glowworm inches forward:\n");
						head++;
						tail++;
						printGlowworm(tail, head, maxLength, death);
					}	
				}	

				// When char 'x', 'X', or '%' is read the worm DIES. SAD.
				else if( (argv[i][j] == 'x') || (argv[i][j] == 'X') || (argv[i][j] == '%') )
				{

					// break statement used to end for loop.
					printf("Glowworm meets its demise. SAD.\n");
					death = 1;
					printGlowworm(tail, head, maxLength, death);
					break;
				}

				// When char is read as anything besides what is listed above
				// the worm will chill.	
				else
				{
					printf("Glowworm chills:\n");
					printGlowworm(tail, head, maxLength, death);
					
				}	
			}
		}
	}
	
	return 0;
}
