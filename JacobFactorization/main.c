#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define sizeOfMatrix 12000

int main(int argc, char *argv[])
{
	int  i=0, j=0, sum=0, licz=0;
	int iter;
	int th_id, nthreads;
	int liczba_watkow , id_watku ;
	double dbsum = 0;

	double* x = malloc(sizeOfMatrix * sizeof(double));
	double* x_old = malloc(sizeOfMatrix * sizeof(double));
	double* Mx = malloc(sizeOfMatrix * sizeof(double));
	double* N = malloc(sizeOfMatrix * sizeof(double));
	int * b = malloc(sizeOfMatrix * sizeof(int *));

	int** A = malloc(sizeOfMatrix * sizeof(int *));
	double ** D = malloc(sizeOfMatrix * sizeof(double *));
	double ** M = malloc(sizeOfMatrix * sizeof(double *));
	double ** sumLU = malloc(sizeOfMatrix * sizeof(double *));

	for(i = 0; i < sizeOfMatrix; i++)
	{
        A[i] = malloc(sizeOfMatrix * sizeof(int));
		D[i] = malloc(sizeOfMatrix * sizeof(double));
		M[i] = malloc(sizeOfMatrix * sizeof(double));
		sumLU[i] = malloc(sizeOfMatrix * sizeof(double));
	    if(A[i]==NULL||D[i]==NULL||M[i]==NULL||sumLU[i]==NULL)
	    {
	    	fprintf(stderr, "out of memory\n");
	    }
	}

    if(b== NULL)
    {
    	fprintf(stderr, "out of memory\n");
    }


	printf("Jacob Factorization\n");

	printf("Generacja macierzy A i b\n");
	#pragma omp parallel private (i,j) shared (A,b,licz,sum)  num_threads (4)
	{
		id_watku = omp_get_thread_num ();
		printf(" Moj watek ma identyfikator : %d\n", id_watku);
		#pragma omp for schedule (static)
		for(i=0;i<sizeOfMatrix;i++)
		{
			b[i]= rand()%10-5;
		}

		// macierz A
		#pragma omp for schedule (static)
		for(j=0; j<sizeOfMatrix; j++)
		{
			for(i=0;i<sizeOfMatrix;i++)
				A[i][j]=0;
		}
		#pragma omp for schedule (static)
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
					// zlicznie liczby wstawianych znakow
					licz++;
				}
			}
		}

		#pragma omp for schedule (static)
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
	}//omp 1
	printf("Wstawilem liczb: %d\n", licz);
	printf("Przygotowanie macierzy D,M,sumLU,Mx,N,x,x_old\n");

	time_t start = time(NULL);
	#pragma omp parallel private (i,j) shared (A,b,D,M,sumLU,x,x_old,N,Mx)  num_threads (4)
	{
		#pragma omp for schedule (static)
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
		#pragma omp for schedule (static)
    	for(i=0;i<sizeOfMatrix;i++)
    		D[i][i] = A[i][i];

		#pragma omp for schedule (static)
    	for(i=0;i<sizeOfMatrix;i++)
    		N[i] = 1/D[i][i];

		#pragma omp for schedule (static)
    	for(i=0;i<sizeOfMatrix;i++)
    		for(j=0;j<sizeOfMatrix;j++)
    			if(i!=j)
    				sumLU[i][j] =A[i][j];

		#pragma omp for schedule (static)
    	for(i=0;i<sizeOfMatrix;i++)
    		for(j=0;j<sizeOfMatrix;j++)
    		{
    			M[i][j]=-(sumLU[i][j]*N[i]);
    		}


    	iter=50;

    	printf("Poczatek obliczen\n");
    	while(iter>0)
    	{
    		for (i=0; i<sizeOfMatrix; i++)
    		{
    			x[i]= N[i]*b[i];
    			for (j=0; j<sizeOfMatrix; j++)
    			{
    				Mx[i] = M[i][j]*x_old[j];
    				x[i] +=Mx[i];
    			//	printf("i=%d, j=%d", i,j);
    			}
    		}
    		for (i=0; i<sizeOfMatrix; i++)
    			x_old[i] = x[i];
    		iter--;
    	}
	}// omp2
    printf("Czas trwania: %.2f\n", (double)(time(NULL) - start));

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
    for(i = 0; i < sizeOfMatrix; i++)
    {
    	free(A[i]);
    	free(D[i]);
    	free(sumLU[i]);
    	free(M[i]);
    }

    free(A);
    free(b);
    free(D);
    free(sumLU);
    free(M);
    free(x);
    free(x_old);
    free(Mx);
    free(N);

    printf("Koniec\n");
	return 0;
}

