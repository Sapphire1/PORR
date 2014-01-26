#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "auxiliaryFunctionsV.h"
#include "choleskyFactorizationV.h"

#define REPEAT 1

void runAlgorithm(double **A, double *b, int problemSize) {
	double *x,*times;
	double meanTime=0, timeSum=0, dbsum=0, SumError=0, *R, meanError = 0;
	int i, j, k;

	times = allocade1DArray(REPEAT);
	R = allocade1DArray(REPEAT);
	x = allocade1DArray(problemSize);//wynik dzialania algorytmu

	/*powtórz algorytm REPEAT razy*/
	for(k=0; k < REPEAT; k++) {
		times[k] = runAlgorithmCholesky(A, b, problemSize, x);

	    for(i=0; i<problemSize; i++)
	    {
	        for(j=0; j<problemSize; j++)
	        {
	            dbsum += A[i][j]*x[j];
	        }

	        R[k] = pow( dbsum-b[i], 2);
	        dbsum=0;
	    }
	}

	/*sumuj czasy*/
	for(i=0; i < REPEAT; i++) {
		timeSum += times[i];
	}

	for(i=0; i < REPEAT; i++) {
		SumError += R[i];
	}

	SumError = sqrt(SumError);

	meanError = SumError/REPEAT;
	meanTime = timeSum/REPEAT;

	printf("Średni czas wykonania algorytmu : %4.8f\n",meanTime);
	printf("Średni błąd : %4.8f \n",meanError);

	free(times);
	free(R);
	free(x);
}

int main()
{
   //macierz wejsciowa m1
   double **m1;
   double *b;
   int action = -1;
   int problemSize = 0;


   printf("\n");
   printf("Ten program rozwiązuje liniowe układy równań\n");
   printf("Układy równań są automatycznie generowane użytkownik podaje tylko rozmiar problemu\n");

   while(1)
   {
	   printf("\nCo chcesz zrobić ?\n");
	   printf("Rozwiąż układ używając metodą Choleskiego (Wersja sekwencyjna) WPISZ: 1\n");
	   printf("Zakończ program WPISZ: 2\n");

       scanf("%d", &action);

       if(action == 1) {
    	   printf("Wpisz rozmiar problemu: \n");
    	   scanf("%d", &problemSize);

    	   m1 = allocate2DArray(problemSize, problemSize);
    	   b = allocade1DArray(problemSize);
    	   matrixGeneration(problemSize, b , m1);

    	   runAlgorithm(m1, b, problemSize);

    	   deallocate2D(m1,problemSize);
    	   free(b);

       }
       else if(action == 2){
    	   break;
       }
       else {
    	   printf("Błędna komenda\n");
       }

   }
    return 0;
}

