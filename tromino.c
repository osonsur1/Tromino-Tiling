#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


int tile = 1;
//structure of matrix with 4 members
struct matstruct
{
	int matrix[100][100];
	int rows;
	int columns;
	int quad;
};

//the matrix of type char is used for printing results
struct charMat
{
	char matrix[100][100];
};

struct matstruct prepMatrix(int m, struct matstruct matD);
void displayMatrix(int m, struct matstruct matD);
int nthpowerTwo(int n);
struct matstruct setHole(int n, struct matstruct board);
struct matstruct trominoTile(int n, struct matstruct board);
struct matstruct getMatrix(int i1, int i2, int j1, int j2, struct matstruct resMat);
struct matstruct getAllocated(int n, struct matstruct board);
struct matstruct joinMatrix(int n, struct matstruct c11, struct matstruct c12, struct matstruct c21, struct matstruct c22);


//main function
int main(int argc, char const *argv[])
{
	int n = atoi(argv[1]);
	int m = nthpowerTwo(n);
	
	struct matstruct *matA = malloc(sizeof(struct matstruct));
	
	matA->rows = atoi(argv[2]);
	matA->columns = atoi(argv[3]);
	
	//check if inputs entered are valid or not
	if(m <= matA -> rows || m<=matA -> columns)
	{
		printf("The position of the hole is out of range!!!!\n\n");
		return;
	}

	//to construct the board and to set the hole at the given position in the board
	struct matstruct resA = prepMatrix(m, *matA);
	struct matstruct board = getAllocated(m, setHole(m, resA));
	
	//call to the function that contains tromino tile algorithm
	struct matstruct res = trominoTile(m, board);
	printf("Tromino tiling:\n\n");
	//displaying matrix	
	displayMatrix(m, res);
	return 0;
}

//constructs the matrix/board
struct matstruct prepMatrix(int m, struct matstruct matD)
{
	int i = 0;
	for(i = 0; i < m; i++)
	{
		int j = 0;
		for (j = 0; j < m; j++)
		{
			matD.matrix[i][j] = 0;
		}
	}
	return matD;
}


// get location of the filled / allocated quadrant
struct matstruct getAllocated(int n, struct matstruct board)
{
	if(board.rows >= 0 && board.rows < (n/2))
	{
		if(board.columns >= 0 && board.columns < (n/2))
		{
			board.quad = 1;	
		}
		else
		{
			board.quad = 2;
		}
	}
	else if(board.rows >= (n/2) && board.rows < n)
	{
		if(board.columns >= 0 && board.columns < (n/2))
		{
			board.quad = 3;	
		}
		else
		{
			board.quad = 4;
		}
		
	}
	return board;
}


//This function contains tromino algorithm
struct matstruct trominoTile(int n, struct matstruct board)
{	
	/*the following code is basically:
	- when value of n is 2, then find value of x and place number in rest
	- return board
	- else find the quadrant then place 3 tiles in the corners of other quadrants
	- divide n/2  four recursive calls and join n/2 matrices into n and return
	*/

	if(2 == n)
	{
		int i = 0;
		while(i < n)
		{
			int j = 0;
			while(j < n)
			{
				if(board.matrix[i][j] == 0)
				{
					if(tile > 9)
					{
						tile = 1;
					}
					board.matrix[i][j] = tile;
				}
				j++;
			}
			i++;
		}
		tile = tile + 1;
		return board;
	}
	else
	{
		if(tile > 9)
		{
			tile = 1;
		}

		//placing the tiles at corners of other quadrants
		if(board.quad == 1)
		{
			//board.matrix[(n/2)-1][(n/2)-1] = tile; //quadrant1
			board.matrix[(n/2)-1][(n/2)] = tile; //quadrant2
			board.matrix[(n/2)][(n/2)-1] = tile; //quadrant3
			board.matrix[(n/2)][(n/2)] = tile; //quadrant4
			tile = tile + 1;
		}

		else if(board.quad == 2)
		{
			board.matrix[(n/2)-1][(n/2)-1] = tile; //quadrant1
			//board.matrix[(n/2)-1][(n/2)] = tile; //quadrant2
			board.matrix[(n/2)][(n/2)-1] = tile; //quadrant3
			board.matrix[(n/2)][(n/2)] = tile; //quadrant4
			tile = tile + 1;
		}

		else if (board.quad == 3)
		{
			board.matrix[(n/2)-1][(n/2)-1] = tile; //quadrant1
			board.matrix[(n/2)-1][(n/2)] = tile; //quadrant2
			//board.matrix[(n/2)][(n/2)-1] = tile; //quadrant3
			board.matrix[(n/2)][(n/2)] = tile; //quadrant4
			tile = tile + 1;
		}

		else
		{
			board.matrix[(n/2)-1][(n/2)-1] = tile; //quadrant1
			board.matrix[(n/2)-1][(n/2)] = tile; //quadrant2
			board.matrix[(n/2)][(n/2)-1] = tile; //quadrant3
			//board.matrix[(n/2)][(n/2)] = tile; //quadrant4
			tile = tile + 1;
		}

		struct matstruct *c = malloc(4 * sizeof(struct matstruct));
		struct matstruct *d = malloc(4 * sizeof(struct matstruct));
		
		c[0] = getAllocated(n/2, getMatrix(0, (n/2), 0, (n/2), board));
		c[1] = getAllocated(n/2, getMatrix(0, (n/2), (n/2), n, board));
		c[2] = getAllocated(n/2, getMatrix((n/2), n, 0, (n/2), board));
		c[3] = getAllocated(n/2, getMatrix((n/2), n, (n/2), n, board));

		/*
		dividing the matrix and making recursive calls to the 
		tromino function
		*/
		d[0] = trominoTile(n/2, c[0]);
		d[1] = trominoTile(n/2, c[1]); 
		d[2] = trominoTile(n/2, c[2]);
		d[3] = trominoTile(n/2, c[3]);

		//call to build the result matrix
		struct matstruct *joinedMatrix = malloc (sizeof(struct matstruct));
		*joinedMatrix = joinMatrix(n, d[0], d[1], d[2], d[3]);
		return *joinedMatrix;
	}


}

//this function sets the hole in the board
struct matstruct setHole(int n, struct matstruct board)
{
	board.matrix[board.rows][board.columns] = 120;
	return board;
}

//the following function prints the given matrix/board
void displayMatrix(int m, struct matstruct matD)
{
	struct charMat *board = malloc(sizeof(struct charMat));
	int a1 = 0;
	for(a1 = 0; a1 < m; a1++)
	{
		int a2 = 0;
		for (a2 = 0; a2 < m; a2++)
		{
			if(matD.matrix[a1][a2] == 120)
			{
				board -> matrix[a1][a2] = 120;
			}
			else
			{
				board -> matrix[a1][a2] = '0' + matD.matrix[a1][a2];
			}
			printf("%c ", board -> matrix[a1][a2]);
		}
		printf("\n");
	}
	printf("\n\n");
}

//this function returns the n th power of two
int nthpowerTwo(int n)
{
	int i = 1, total = 1;
	while(i <= n)
	{
		total = total * 2;
		i++;
	}
	return total;
}

//this function returns a sub matrix of the given matrix
struct matstruct getMatrix(int i1, int i2, int j1, int j2, struct matstruct resMat)
{
	struct matstruct *addRes = malloc(sizeof(struct matstruct));
	int i = i1, m = 0;
	for(i = i1; i < i2; i++)
	{
		int j = 0, n = 0;
		for (j = j1; j < j2; j++)
		{
			if(resMat.matrix[i][j] != 0)
			{
				addRes -> rows = m;
				addRes -> columns = n;
			}
			addRes -> matrix[m][n] = resMat.matrix[i][j];
			n = n + 1;
		}
		m++;
	}
	struct matstruct board = getAllocated(m, *addRes);
	return board;
} 

//this function builds matrix and returns it by compiling given matrices
struct matstruct joinMatrix(int n, struct matstruct c11, struct matstruct c12, struct matstruct c21, struct matstruct c22)
{
	struct matstruct *addRes = malloc(sizeof(struct matstruct));
	int p = 0, q = 0;
	//for c-11
	//int i1 = 0;
	for(int i1 = 0; i1 < n/2; i1++)
	{
		int j1 = 0, q = 0;
		while(j1 < n/2)
		{
			addRes -> matrix[p][q] = c11.matrix[i1][j1];
			q = q+1;
			j1++;
		}
		p = p + 1;
	}

	//for c-21
	p = n/2;
	q = 0;
	int i2 = 0;
	for(i2 = 0; i2 < n/2; i2++)
	{
		int j2 = 0, q = 0;
		while(j2 < n/2)
		{
			addRes -> matrix[p][q] = c21.matrix[i2][j2];
			q = q + 1;
			j2++;
		}
		p = p + 1;
	}

	//for next half
	p = 0;
	q = (n/2);

	//for c-12
	int i3 = 0;
	for(i3 = 0; i3 < n/2; i3++)
	{
		int j3 = 0, q = n/2;
		while(j3 < n/2)
		{
			addRes -> matrix[p][q] = c12.matrix[i3][j3];
			q = q + 1;
			j3++;
		}
		p = p + 1;
	}

	//for c-22
	p = n/2;
	q = n/2;
	int i4 = 0;
	for(i4 = 0; i4 < n/2; i4++)
	{
		int j4 = 0, q = n/2;
		while(j4 < n/2)
		{
			addRes -> matrix[p][q] = c22.matrix[i4][j4];
			q = q + 1;
			j4++;
		}
		p = p + 1;
	}

	return *addRes;
}
