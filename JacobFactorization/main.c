#include <stdio.h>
#include <stdlib.h>
#define sizeOfMatrix 10000

int main(void)
{
	int  i=0, j=0, sum=0, licz=0;
	int iter;
	//double D[sizeOfMatrix][sizeOfMatrix];
	double ** D = malloc(sizeOfMatrix * sizeof(double *));

	    for(i = 0; i < sizeOfMatrix; i++)
	    {
	        D[i] = malloc(sizeOfMatrix * sizeof(double));
	        if(D[i] == NULL)
	        {
	            fprintf(stderr, "out of memory\n");
	        }
	    }
	//double b[sizeOfMatrix];// = {30,0,-10,5,2 ,4 ,5 ,6 ,3 ,-2 ,3 ,2 };
	double x[sizeOfMatrix];
	double x_old[sizeOfMatrix];
	double Mx[sizeOfMatrix];
	double N[sizeOfMatrix];
	//double M[sizeOfMatrix][sizeOfMatrix];
	double ** M = malloc(sizeOfMatrix * sizeof(double *));

		    for(i = 0; i < sizeOfMatrix; i++)
		    {
		        M[i] = malloc(sizeOfMatrix * sizeof(double));
		        if(M[i] == NULL)
		        {
		            fprintf(stderr, "out of memory\n");
		        }
		    }
	//double sumLU[sizeOfMatrix][sizeOfMatrix];
	double ** sumLU = malloc(sizeOfMatrix * sizeof(double *));

		    for(i = 0; i < sizeOfMatrix; i++)
		    {
		    	sumLU[i] = malloc(sizeOfMatrix * sizeof(double));
		        if(sumLU[i] == NULL)
		        {
		            fprintf(stderr, "out of memory\n");
		        }
		    }
	double dbsum = 0;
	//int A [sizeOfMatrix][sizeOfMatrix] ;

	//double A[sizeOfMatrix][sizeOfMatrix] = {{4,-1,-0.2,2},{-1,5,0,-2},{0.2,1,10,-1},{0,-2,-1,4}};

	int **A;
    A = malloc(sizeOfMatrix * sizeof(int *));

    for(i = 0; i < sizeOfMatrix; i++)
    {
        A[i] = malloc(sizeOfMatrix * sizeof(int));
        if(A[i] == NULL)
        {
            fprintf(stderr, "out of memory\n");
        }
    }
    int * b = malloc(sizeOfMatrix * sizeof(int *));
    if(b== NULL)
    {
    	fprintf(stderr, "out of memory\n");
    }


	printf("Jacob Factorization\n");

	// przygotowanie danych wejsciowych
	// wektor b

	// wektor arrayB
	for(i=0;i<sizeOfMatrix;i++)
	{
		b[i]= rand()%10-5;
	}
	for(i=0;i<sizeOfMatrix;i++)
	{
//		printf("arrayB[i]= %d\t", b[i]);
	}
	// macierz A
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
    //			printf("Wstawiam: %d\n", A[i][j]);
    			//ile pozycji przesuwamy sie w wierszu nie wpisujac nic do niego
    			i+=(rand()%(sizeOfMatrix/3))+1;
    			licz++;
    		}
    	}
    }
    for(j=0; j<sizeOfMatrix; j++)
    {
    	for(i=0;i<sizeOfMatrix;i++)
        {
    		sum +=A[i][j];
        }
   // 	printf("Suma: %d\n", sum);
    	// na diagonali niech będzie od 1 do 19 więcej niż suma w wierszu, wtedy bedzie macierz dominowala diagonalnie wierszowo
        A[j][j]=sum+rand()%20+1;
   //     printf("Wstawiam na diagonale: %d\n", A[j][j]);
        sum=0;
    }

    for(j=0; j<sizeOfMatrix; j++)
    {
    //   	for(i=0;i<sizeOfMatrix;i++)
   //    		printf("%d\t", A[i][j]);
    //   	printf("\n");
	}

    printf("Wstawilem liczb: %d\n", licz);

    for(i=0;i<sizeOfMatrix;i++)
    {
    	for(j=0;j<sizeOfMatrix;j++)
    	{
    		D[i][j] = 0;
    		M[i][j] = 0;
    		sumLU[i][j] = 0;
        }
		x[i]=0;
		x_old[i]=0;
		Mx[i]=0;
		N[i] = 0;
		x_old[i]=0;
    }
    iter=0;

    for(i=0;i<sizeOfMatrix;i++)
        D[i][i] = A[i][i];


    for(i=0;i<sizeOfMatrix;i++)
    	N[i] = 1/D[i][i];

    for(i=0;i<sizeOfMatrix;i++)
         for(j=0;j<sizeOfMatrix;j++)
         	if(i!=j)
         		sumLU[i][j] =A[i][j];

    for(i=0;i<sizeOfMatrix;i++)
    	for(j=0;j<sizeOfMatrix;j++)
    	{
    		M[i][j]=-(sumLU[i][j]*N[i]);
    	}


    iter=50;

    while(iter>0)
    {
       for (i=0; i<sizeOfMatrix; i++)
       {
          x[i]= N[i]*b[i];
          for (j=0; j<sizeOfMatrix; j++)
          {
        	 Mx[i] = M[i][j]*x_old[j];
             x[i] +=Mx[i];
          }
       }
       for (i=0; i<sizeOfMatrix; i++)
          x_old[i] = x[i];
       iter--;
    }

    printf("Wynik\n");
    //for (i=0; i<sizeOfMatrix; i++)
   //    printf("x[%d] = %f\n", (i+1), x_old[i]);

    printf("Sprawdzam!");
    for(j=0;j<sizeOfMatrix;j++)
    {
    	for(i=0;i<sizeOfMatrix;i++)
    		dbsum += A[i][j]*x[i];
    //	printf("Wyszlo = %.15f, a mialo wyjsc %d\n", dbsum, b[j]);
    	dbsum=0;
    }
    free(A);
    free(b);
    free(D);
    free(sumLU);
    free(M);
    printf("Koniec\n");
	return 0;
}

