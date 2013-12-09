#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>


#define sizeOfMatrix 10000

void generateMatrixA(double** A, double* b)
{
	int i,j, licz=0, sum=0;
	//	id_watku = omp_get_thread_num ();
	//	printf(" Moj watek ma identyfikator : %d\n", id_watku);
		for(i=0;i<sizeOfMatrix;i++)
		{
			b[i]= rand()%10-5;
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
					//srand (time(NULL));
					i+=(rand()%(sizeOfMatrix/3))+1;
					// zlicznie liczby wstawianych znakow
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
			// na diagonali niech będzie od 1 do 19 więcej niż suma w wierszu, wtedy bedzie macierz dominowala diagonalnie wierszowo
			//srand (time(NULL));
			A[j][j]=sum+rand()%20+1;
			sum=0;
		}
	printf("Wstawilem liczb: %d\n", licz);
	return ;
}

void jacobFactorization(double** A, double** D, double** M, double** sumLU, double* x, double* x_old, double* Mx, double* N, double* b)
{
	int i,j,iter=50;
	printf("Faktoryzacja\n");

	#pragma omp parallel private (i,j,iter) shared (A,b,D,M,sumLU,x,x_old,N,Mx)  num_threads (4)
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

			printf("Poczatek obliczen\n");
	    	    	for(iter=50; iter>0;iter--)
	    	    	{
	    			#pragma omp for schedule (static)
	    	    		/*cilk_for*/for(i=0; i<sizeOfMatrix; i++)
	    	    		{
	    	    			x[i]= N[i]*b[i];
					//#pragma omp for schedule (static)
	    	    			for(j=0; j<sizeOfMatrix; j++)
	    	    			{
	    	    				Mx[i] = M[i][j]*x_old[j];
	    	    				x[i] +=Mx[i];
	    	    			//	printf("i=%d, j=%d", i,j);	
	    	    			}
	    	    		}
	    	    		//#pragma omp for schedule (static)
	    	    		/*cilk_for*/for(i=0; i<sizeOfMatrix; i++)
	    	    			x_old[i] = x[i];
	    	    	}
	    }// omp2
}

int main(int argc, char *argv[])
{
	printf("Jacob Factorization\n");
	int  i=0, j=0;
	double dbsum = 0, error = 0, bladKoncowy=0, bladLaczny=0;
	double czasLaczny=0;
	printf("Inicjalizacja macierzy\n");

	double* x = malloc(sizeOfMatrix * sizeof(double));
	double* x_old = malloc(sizeOfMatrix * sizeof(double));
	double* Mx = malloc(sizeOfMatrix * sizeof(double));
	double* N = malloc(sizeOfMatrix * sizeof(double));
	double * b = malloc(sizeOfMatrix * sizeof(double*));

	double** A = malloc(sizeOfMatrix * sizeof(double *));
	double ** D = malloc(sizeOfMatrix * sizeof(double *));
	double ** M = malloc(sizeOfMatrix * sizeof(double *));
	double ** sumLU = malloc(sizeOfMatrix * sizeof(double *));

	int counter=0;

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

    for (counter=0;counter<10;counter++)
    {
    	printf("Iteracja %d\n", counter+1);
		printf("Generacja macierzy A i b\n");
		generateMatrixA(A, b);
		printf("Przygotowanie macierzy D,M,sumLU,Mx,N,x,x_old\n");

		time_t start = time(NULL);
		jacobFactorization(A, D, M, sumLU, x, x_old, Mx, N, b);
		czasLaczny+=(double)(time(NULL) - start);
	    printf("Czas trwania: %.2f\n", (double)(time(NULL) - start));



		printf("Sprawdzam!");
		for(j=0;j<sizeOfMatrix;j++)
		{
			for(i=0;i<sizeOfMatrix;i++)
			{
				dbsum += A[i][j]*x[i];
			}
			error += pow( dbsum-b[j], 2);
			dbsum=0;
		}
		bladLaczny+=bladKoncowy=sqrt(error);
		printf("Blad : %f\n", bladKoncowy);
		if(bladKoncowy>0.1)
			 printf("Blad obliczen jest za duzy!\n");
	}//for
    printf("Sredni czas : %f\n", czasLaczny/counter);
    printf("Sredni blad : %f\n", bladLaczny/counter);

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

