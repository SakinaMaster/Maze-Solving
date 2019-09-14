/* Class: CS 211
   Author: Sakina Master
   Date:   June 01 2018 */
   
#include <stdio.h>		//For printf and scanf
#include <stdlib.h>		//For using malloc()

//typical C boolean set-up
#define TRUE   1
#define FALSE  0

typedef struct mazeStruct
{
    char** darr;				//2d dynamic array
    int xsize, ysize;			//Size of rows and columns
    int xstart, ystart;			//Start indices for x and y co-ordinates
    int xend, yend;				//End indices for x and y co-ordinates
}maze;

typedef struct linkedNode
{
    int xElem;					//x co-ordinate
    int yElem;					//y co-ordinate
    struct linkedNode* next;	// Pointer structure that points to next structure in linked list.
}linkedNode;

typedef linkedNode *pNode; 		//Pointer to a struct


//-----------------------------------------------------------------------------------------------------------
//Function which initializes the maze. It reads the file, allocates memory for 2d array,
//gets all the variable values for mazeStruct, does input validation, and displays the maze.
//It takes the address of the maze node and the FILE* as parameters. It does not return anything.
void initMaze(maze* m1, FILE *src)
{
    /* read in the size, starting and ending positions in the maze */
    int xpos;
    int ypos;
    while(!feof(src))    {
        fscanf (src, "%d %d", &m1->xsize, &m1->ysize);
        if((m1->xsize < 1 ) || (m1->ysize < 1))				//Checks if the size is valid.
        {
            fprintf(stderr, "Invalid: Maze sizes must be greater than 0\n");
            continue;		//If the size is not valid read the next line and get the size.
        }
        else
        {
            break;			//Breaks the while loop when a valid size is found.
        }
    }
    
    while(!feof(src))    {
        fscanf (src, "%d %d", &m1->xstart, &m1->ystart);
        if((m1->xstart > m1->xsize) || (m1->xstart < 1))	//Checks if the start position for x and y are valid
        {
            fprintf(stderr, "Invalid: row %d is outside range from 1 to %d\n", m1->xstart, m1->xsize);
            continue;		//If the start position for x is not valid read the next line.
        }
        if((m1->ystart > m1->ysize) || (m1->ystart < 1))
        {
            fprintf(stderr, "Invalid: column %d is outside range from 1 to %d\n", m1->ystart, m1->ysize);
            continue;		//If the start position for y is not valid read the next line.
        }
        else
        {
            break;			//Break the while loop when valid x and y start positions are found.
        }
    }
    
    while(!feof(src))    {
        fscanf (src, "%d %d", &m1->xend, &m1->yend);
        if((m1->xend > m1->xsize) || (m1->xend < 1))	//Checks if the end position for x and y are valid
        {
            fprintf(stderr, "Invalid: row %d is outside range from 1 to %d\n", m1->xend, m1->xsize);
            continue;		//If the end position for x is not valid read the next line.
        }
        if((m1->ystart > m1->ysize) || (m1->ystart < 1))
        {
            fprintf(stderr, "Invalid: column %d is outside range from 1 to %d\n", m1->ystart, m1->ysize);
            continue;		//If the end position for y is not valid read the next line.
        }
        else
        {
            break;			//Break the while loop when valid x and y end positions are found.
        }
    }
    
    /* print them out to verify the input */
    printf ("size: %d, %d\n", m1->xsize, m1->ysize);
    printf ("start: %d, %d\n", m1->xstart, m1->ystart);
    printf ("end: %d, %d\n", m1->xend, m1->yend);
    
    int i, j;
    m1->darr = (char**)malloc(sizeof(char*) * (m1->xsize + 2));	//Allocating space for rows.
    for(i = 0; i < m1->xsize + 2; i++){
        m1->darr[i] = (char*) malloc(sizeof(char) * (m1->ysize + 2));//Allocating memeory space for each column.
    }
	// initialize the maze to empty 
    for (i = 0; i < m1->xsize+2; i++)
        for (j = 0; j < m1->ysize+2; j++)
            m1->darr[i][j] = '.';
    
    // mark the borders of the maze with *'s 
    for (i=0; i < m1->xsize+2; i++)
    {
        m1->darr[i][0] = '*';
        m1->darr[i][m1->ysize+1] = '*';
    }
    for (i=0; i < m1->ysize+2; i++)
    {
        m1->darr[0][i] = '*';
        m1->darr[m1->xsize+1][i] = '*';
    }
    
    // mark the starting and ending positions in the maze 
    m1->darr[m1->xstart][m1->ystart] = 's';
    m1->darr[m1->xend][m1->yend] = 'e';
    
    // mark the blocked positions in the maze with *'s and make sure they don't block starting and ending positions.
    while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
        if(    m1->darr[xpos][ypos] == 's')
        {
            fprintf(stderr,"Invalid: attempting to block starting position\n");
            continue;
        }
        if(m1->darr[xpos][ypos] == 'e')
        {
            fprintf(stderr,"Invalid: attempting to block ending position\n");
            continue;
        }
        m1->darr[xpos][ypos] = '*';
    }
    
    // print out the initial maze 
    for (i = 0; i < m1->xsize+2; i++)
    {
        for (j = 0; j < m1->ysize+2; j++)
            printf ("%c", m1->darr[i][j]);
        printf("\n");
    }
}


//-----------------------------------------------------------------------------------------------
//Pushes all locations of maze in head of linked list. It does not return anything.
void push(pNode *head, int xVal, int yVal, int debugMode)
{
    pNode temp = (pNode) malloc (sizeof(linkedNode)); //Allocate space for a new node
    temp -> xElem = xVal; 	//Store the value of x
    temp -> yElem = yVal;	//Store the value of y
    if(debugMode == TRUE)
    {
    	printf("Pushing in Stack: (%d, %d)\n",temp -> xElem, temp -> yElem);
	}
    temp -> next = *head;	//Make the next pointer point to head which is null.
    *head = temp;			//The new node just created becomes the head of the list.
}


//---------------------------------------------------------------------------------------------------------
//This function removes the top node off the stack and frees its memeory.
void pop (pNode *head, int debugMODE)
{
    pNode temp = *head;		//Store the head of the list in a temp pointer.
    if(debugMODE == TRUE)
    {
    	printf("Popping off stack: (%d, %d)\n", temp->xElem, temp->yElem);
	}
    *head = (*head)->next;	//Make the next node in the list as the head of the node.
    free (temp);			//Free the memory of the node, which head was originally pointing to.
}


//----------------------------------------------------------------------------------------------------------
//This function initializes the linkedNode struct by pushing the start element in the stack.
//This function does not return anything.
void init(pNode *head, int xStart, int yStart, int debugMODE)
{
    push(head, xStart, yStart, debugMODE); //Calling the push()
}


//----------------------------------------------------------------------------------------------------------
//Checks if the stack is empty. This function takes pNode head as its parameter and returns an integer.
int stackIsEmpty (pNode head)
{
    if (head == NULL)		//Checks if head is NULL.
        return TRUE;
    else
        return FALSE;
}


//-----------------------------------------------------------------------------------------------------------
//This function shows the co-ordinates of the solution path starting from the start of the maze to the end of the maze.
//This function does not return anything.
void showSolution(pNode head)
{
    while(head != NULL)
    {
        printf("(%d , %d)\n", head->xElem, head->yElem);
        head = head->next;		//Make the head point to the next node.
    }
}


//------------------------------------------------------------------------------------------------------------
//Returns the top node of the stack by returning head. Since we prepend nodes, the head always points to the top.
linkedNode top(pNode head)
{
    return *head;
}


//------------------------------------------------------------------------------------------------------------
//This function returns TRUE if the x and y co-ordinates are equal to the end positions.
int endFound(pNode *head, int xend, int yend)
{
    pNode temp = *head;
    if((temp->xElem == xend) && (temp->yElem == yend))	//Checks if the x and y co-ordinates are same as end positions. 
    {
        return TRUE;										
    }
    else
    {
        return FALSE;
    }
}


//--------------------------------------------------------------------------------------------------------------
//Frees the dynamically allocated char  2d array.
void resetArray(maze *m1)
{
	int i;
	for(i=0; i<m1->xsize+2; i++)
	{
		free(m1->darr[i]);
	}
	free(m1->darr);
}


//----------------------------------------------------------------------------------------------------------------
//Frees the stack at the end of the program.
void resetStack(pNode *head)
{
	pNode temp = *head;
	pNode curr;
	while(temp!= NULL)
	{
		curr = temp->next;
		free(temp);
		temp = curr;
	}
	*head = temp;
	free(temp);
}


int main (int argc, char **argv)
{
    maze m1;				//Node for creating maze.
    pNode head = NULL;		//Declaring and initializing head ptr of the linked list to NULL.
    int debugMODE;		    // Variable to set up the debug mode.

  // Setting up the DEBUG MODE.
	int i;
 	for ( i = 0 ; i < argc ; i++ )
	{
		if (( argv[i][0] == '-') && (argv[i][1] == 'd'))	//If user enters '-d' in command line argument, the program runs in debug mode.
		{
			debugMODE = TRUE;
		}
	}
	
 	FILE *src;					//Declare a pointer to FILE

	// verify the proper number of command line arguments were given 
 	if(argc > 3) {
        printf("Too many arguments were given");
        exit(-1);
    }

	if(argc == 3) {
    	if(strcmp(argv[1], "-d") == 0) {
    		if ( ( src = fopen( argv[2], "r" )) == NULL ) {
        		printf ( "Can't open input file: %s", argv[2] );
        		exit(-1);
    		}
		}
    else{
		if(strcmp(argv[2], "-d") == 0) {
			if ((src = fopen( argv[1], "r" )) == NULL ) {
        		printf ( "Can't open input file: %s", argv[1] );
        		exit(-1);
    		}
		}
		}	
	}
 
 	if(argc == 2) {
		if ((src = fopen( argv[1], "r" )) == NULL ) {
        	printf ( "Can't open input file: %s", argv[1] );
        	exit(-1);
    	}
	}
	
	if(argc == 1) {
        printf("Usage: %s <input file name>\n", argv[0]);
        exit(-1);
    }
    
    initMaze(&m1, src);
    init(&head, m1.xstart, m1.ystart, debugMODE);
    linkedNode topValue = top(head);			//Calling top()
    m1.darr[head->xElem][head->yElem] = 'V';	//Mark the array position visited
   	topValue = top(head);
   	
	//If the stack is not empty or the end is not found enter while loop
	while((stackIsEmpty(head) != TRUE) || (endFound(&head, m1.xend, m1.yend)) == FALSE)
    {
        topValue = top(head);
        //Check if the end co-ordinates of x and y are found.
		if(((head->xElem == m1.xend)) && ((head->yElem == m1.yend)))
        {
            printf("\nEnd has been found\n");
            break;
        }
        //Checks the right neighbor. 
        if((m1.darr[head->xElem][head->yElem+1] == '.') || (m1.darr[head->xElem][head->yElem + 1] == 'e'))
        {
            m1.darr[head->xElem][head->yElem+1] = 'V';
            push(&head,head->xElem, head->yElem+1, debugMODE );
        }
        //Checks the bottom neighbor.
        else if((m1.darr[head->xElem+1][head->yElem] == '.') || (m1.darr[head->xElem+1][head->yElem] == 'e'))
        {
            m1.darr[head->xElem+1][head->yElem] = 'V';
            push(&head,head->xElem+1, head->yElem, debugMODE );
        }
        //Checks the left neighbor.
        else if((m1.darr[head->xElem][head->yElem-1] == '.') || (m1.darr[head->xElem][head->yElem-1] == 'e'))
        {
            m1.darr[head->xElem][head->yElem-1] = 'V';
            push(&head,head->xElem, head->yElem-1, debugMODE );
        }
        //Checks the above neigbor.
        else if((m1.darr[head->xElem-1][head->yElem] == '.') || (m1.darr[head->xElem-1][head->yElem] == 'e'))
        {
            m1.darr[head->xElem-1][head->yElem] = 'V';
            push(&head,head->xElem-1, head->yElem, debugMODE );
        }
        else
        {
            pop(&head, debugMODE); //Pops the top node
            if(stackIsEmpty(head) == TRUE){
                printf("\nThere is no solution.\n");
                break;
            }
            else if((endFound(&head, m1.xend, m1.yend)) == TRUE){
                break;
            }
        }
    }//end while
    
    if (stackIsEmpty(head) == FALSE)
    {
        printf("The Solution path is: \n");
        showSolution(head);	//Displays the solution path
    }
	resetArray(&m1);		//Reset dynamically allocated 2d array
	resetStack(&head);		//Reset Stack
    return 0;
}//end main()
