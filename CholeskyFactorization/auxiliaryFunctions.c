#include <stdlib.h>
#include <stdio.h>

void print2DArray(double **A, int rowsNum,int colsNum)
{
    printf("\n");
    int m,n;

    for (m=0;m<rowsNum;m++)
    {
        for (n=0;n<colsNum;n++)
        {
            printf("[%d,%d]= %.5f ",m,n,A[m][n]);
        }
        printf("\n");
    }
}

void print1DArray(double *A, int size)
{
    printf("\n");
    int m;

	for (m=0 ;m < size; m++)
	{
		printf("[%d]= %.5f ",m, A[m]);
	}

    printf("\n");

}

double *allocade1DArray(int size) {
    double *array = calloc(size, sizeof(double));
    return array;
}


/*funkcja alokująca dwuwymiarową tablicę*/
double **allocate2DArray(int numRows, int numCols) {
	double **newArray;
	int i=0;

	newArray = calloc(numRows, sizeof(double*));
	if(newArray == NULL)
	{
		printf("Program nie może zaalokować pamięci\n");
		return NULL;
	}
	for(i = 0; i < numRows; i++)
		{
		newArray[i] = calloc(numCols, sizeof(double));
		if(newArray[i] == NULL)
		{
			printf("Program nie może zaalokować pamięci\n");
			return NULL;
		}
	}

	return newArray;
}

void deallocate2D(double** array, int numRows) {
	/*  deallocate each row  */
	int i;
	for(i = 0; i < numRows; i++) {
		free(array[i]);
	}

	/*  deallocate array of pointers  */
	free(array);
}

void checkSolution(double** matrixA, double* matrixX, double* matrixB, int size) {
	int i, j;
	double temSum = 0;


	for(i = 0; i < size; i++) {
		temSum = 0;
		for(j = 0; j < size; j++) {
			temSum += matrixA[i][j] * matrixX[j];
		}
		printf("%f == %f \n",temSum,matrixB[i]);
	}
}

void matrixGeneration(int sizeOfMatrix, int *b, int **A) {
	int i,j, sum=0, licz=0;
    b = malloc(sizeOfMatrix * sizeof(int *));

    A = malloc(sizeOfMatrix * sizeof(int *));

	printf("Generacja macierzy A i b\n");

		for(i=0;i<sizeOfMatrix;i++)
		{
			b[i]= rand()%10-5;
		}

		for(j=0; j<sizeOfMatrix; j++)
		{
			for(i=0;i<sizeOfMatrix;i++)
					A[i][j]=0;
		}

		for(j=0; j<sizeOfMatrix; j++)
		{
			for(i=j+1;i<sizeOfMatrix;i++)
			{
				if(i!=j)
				{
						A[j][i]=A[i][j] = rand()%10;
						//                        printf("Wstawiam: %d\n", A[i][j]);
						//ile pozycji przesuwamy sie w wierszu nie wpisujac nic do niego
						i+=(rand()%(sizeOfMatrix/3))+1;
						// zlicznie liczby wstawianych znakow
						licz++;
				}
			}
		}

//        #pragma omp for schedule (static)
		for(j=0; j<sizeOfMatrix; j++)
		{
			for(i=0;i<sizeOfMatrix;i++)
			{
					sum +=A[i][j];
			}
			// na diagonali niech będzie od 1 do 19 więcej niż suma w wierszu, wtedy bedzie macierz dominowala diagonalnie wierszowo
			A[j][j]=sum+rand()%20+1;
			sum=0;
		}

}
