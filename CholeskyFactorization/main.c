#include <stdlib.h>
#include <stdio.h>
#include "choleskyFactorization.h"
#include "choleskyFactorizationParallel.h"/*openMP*/
#include "auxiliaryFunctions.h"

int main()
{
   //macierz wejsciowa
   int rowNum = 3;
   double **m1 = allocate2DArray(rowNum, rowNum);

   m1[0][0] = 25;
   m1[0][1] = 15;
   m1[0][2] = -5;

   m1[1][0] = 15;
   m1[1][1] = 18;
   m1[1][2] = 0;

   m1[2][0] = -5;
   m1[2][1] = 0;
   m1[2][2] = 11;

   double *b = allocade1DArray(rowNum);

   b[0] = 1;
   b[1] = 0;
   b[2] = 4;

   /*
	25  15  -5                 5   0   0
	15  18   0         -->     3   3   0
	-5   0  11                -1   1   3
	*/

   int action = -1;
   int problemSize = 0;
   double timeConsumed;

   printf("\n");
   printf("Ten program rozwiązuje liniowe układy równań\n");
   printf("Układy równań są automatycznie generowane użytkownik podaje tylko rozmiar problemu\n");

   while(1)
   {
	   printf("\nCo chcesz zrobić ?\n");
	   printf("Rozwiąż układ używając metodą Choleskiego WPISZ: 1\n");
	   printf("Rozwiąż układ używając metodą Choleskiego (OpenMP) WPISZ: 2\n");
	   printf("Rozwiąż układ używając metodą Choleskiego (Inna metoda) WPISZ: 3\n");
	   printf("Zakończ program WPISZ: 4\n");

       scanf("%d", &action);

       if(action == 4){
    	   break;
       }
       else {
    	   printf("Wpisz rozmiar problemu: \n");
       }

       scanf("%d", &problemSize);

       if(action == 1) {
    	   //tu powinna być generacja macierzy b i A
    	   //int *b,
    	   //int **A
    	   //jest już napisana funkcja
    	   //matrixGeneration(int sizeOfMatrix, int *b, int **A)

    	   timeConsumed = runAlgorithmCholesky(m1, b, problemSize);

    	   printf("Czas wykonania algorytmu : %4.5f ",timeConsumed);
       }
       else if(action == 2) {
    	   //tu powinna być generacja macierzy b i A
    	   //int *b,
    	   //int **A
    	   //jest już napisana funkcja
    	   //matrixGeneration(int sizeOfMatrix, int *b, int **A)

    	   timeConsumed = runAlgorithmCholeskyParallel(m1, b, problemSize);

    	   printf("Czas wykonania algorytmu : %4.5f ",timeConsumed);
       }
       else if(action == 3) {
    	   printf("Jeszcze nie zaimplementowane\n");
       }
       else
       {
           printf("Niepoprawny wpis\n");
       }
   }
    return 0;
}



