#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>


//#define sizeOfMatrix 4096
#define sizeOfMatrix 4000
#define iterations 50
void jacobFactorization(double** A, double** D, double** M, double** sumLU, double* x, double* x_old, double* Mx, double* N, double* b, int startRow, int endRow)
{
	int i,j;
	printf("Faktoryzacja\n");
		
	    	for(i=startRow;i<endRow;i++)
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
		
	    	for(i=startRow;i<endRow;i++)
	    		D[i][i] = A[i][i];

		
	    	for(i=startRow;i<endRow;i++)
	    		N[i] = 1/D[i][i];

		
	    	for(i=startRow;i<endRow;i++)
	    		for(j=0;j<sizeOfMatrix;j++)
	    			if(i!=j)
				    sumLU[i][j] =A[i][j];

		
	    	for(i=startRow;i<endRow;i++)
	    		for(j=0;j<sizeOfMatrix;j++)
	    		{
	    			M[i][j]=-(sumLU[i][j]*N[i]);
	    		}
}
void generateMatrixA(double** A, double* b)
{
		int i,j, licz=0, sum=0;

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
			A[j][j]=sum+rand()%20+1;
			sum=0;
		}
		printf("Wstawilem liczb: %d\n", licz);
		return ;
}


int main(int argc, char *argv[])
{
	int counter=0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int numprocs, rank, namelen;
	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
	int  i=0, j=0;
	double dbsum = 0, error = 0, bladKoncowy=0, bladLaczny=0;
	double czasLaczny=0;
	int received, sended;
	int rows2Fill;
	int temp=0,iter=50;

	
	
	double* x = (double*)malloc(sizeOfMatrix * sizeof(double));
	double* x_old = (double*)malloc(sizeOfMatrix * sizeof(double));
	double* Mx = (double*)malloc(sizeOfMatrix * sizeof(double));
	double* N = (double*)malloc(sizeOfMatrix * sizeof(double));
	double * b = (double*)malloc(sizeOfMatrix * sizeof(double*));
	double** A = (double**)malloc(sizeOfMatrix * sizeof(double *));
	double ** D = (double**)malloc(sizeOfMatrix * sizeof(double *));
	double ** M = (double**)malloc(sizeOfMatrix * sizeof(double *));
	double ** sumLU = (double**)malloc(sizeOfMatrix * sizeof(double *));
	double** z;
	double* x_temp = (double*) malloc(sizeOfMatrix*sizeof(double));
	int limit;
	//double* x_temp =(double*)malloc((sizeOfMatrix/numprocs) * sizeof(double));
	int t=0;
	for(i = 0; i < sizeOfMatrix; i++)
	{
		A[i] = (double*)malloc(sizeOfMatrix * sizeof(double));
		D[i] = (double*)malloc(sizeOfMatrix * sizeof(double));
		M[i] = (double*)malloc(sizeOfMatrix * sizeof(double));
		sumLU[i] = (double*)malloc(sizeOfMatrix * sizeof(double));
		if(A[i]==NULL||D[i]==NULL||M[i]==NULL||sumLU[i]==NULL)
		{
		    fprintf(stderr, "out of memory\n");
		}
	}
	if(b== NULL)
	{
		fprintf(stderr, "out of memory\n");
	}
	if (rank == 0) 
	{
		printf("Master\n");
		for (counter=0;counter<1;counter++)
		{
			rows2Fill = sizeOfMatrix/numprocs;
			generateMatrixA(A, b);
			for(i=0;i<sizeOfMatrix;i++)
			{
			  for(j=0;j<sizeOfMatrix;j++)
			    printf("A[%d][%d]=%f\t", i,j, A[i][j]);
			
			 printf("\n");
			}
			for(i = 1; i < numprocs; i++) 
			{	
				sended=0;
				// przesylanie wiersz po wierszu... moze sprobowac przeslac wszystkie na raz
				while(sended<rows2Fill)
				{
					MPI_Send(A[i*rows2Fill+sended], sizeOfMatrix, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
					sended++;
				}

				MPI_Send(b, sizeOfMatrix, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
			
			// dane sa przeslane dobrze, teraz trzeba zrobic faktoryzacje we wszystkich watkach
			jacobFactorization(A, D, M, sumLU, x, x_old, Mx, N, b,0,rows2Fill);
			for(iter=iterations;iter>0;iter--)
			{
				for(i=0;i<rows2Fill;i++)
				{
				    x[i]= N[i]*b[i];
					
				    for(j=0; j<sizeOfMatrix; j++)
				    {
					Mx[i] = M[i][j]*x_old[j];
					x[i] +=Mx[i];
				    }
				}
				for(i=0;i<rows2Fill;i++)
				{
				    x_old[i] = x[i];
				}		
				//printf("Czekanie, kolejny obieg w master\n");
				for(i = 1; i < numprocs; i++) 
				{
					printf("Kolejna iteracja dla procesu %d\n", i);
					MPI_Recv(x_temp, sizeOfMatrix, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
					for(j=0; j<sizeOfMatrix;j++)
					  printf("x_temp[%d]%f\t", j, x_temp[j]);
					printf("\n");
					// przepisujemy dane z innych procesow
					for(j=i*rows2Fill;j<(i+1)*rows2Fill;j++)
					{	
					      printf("Przepisuje to %f\n", x_temp[j]);
					      x_old[j]=x_temp[j];
					}
				}
				for(j=0; j<sizeOfMatrix;j++)
					  printf("x_old_master[%d]%f\t", j, x_old[j]);
				printf("\n");
				// tutaj wyslanie wartosci calego wektora x_old
				
				for(i = 1; i < numprocs; i++) 
					MPI_Send(x_old, sizeOfMatrix, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				printf("Sprawdzam!");
				for(j=0;j<sizeOfMatrix;j++)
				{
					for(i=0;i<sizeOfMatrix;i++)
					{
						dbsum += A[i][j]*x_old[i];
					}
					printf("Suma wyszla %f, a powinno byc %f", dbsum, b[j]);
					error += pow( dbsum-b[j], 2);
					printf("error = %f\n", error);
					dbsum=0;
				}
				bladKoncowy=sqrt(error);
				error=0;
				bladLaczny+=bladKoncowy;
				printf("Blad : %f\n", bladKoncowy);
				if(bladKoncowy>0.1)
				    printf("Blad obliczen jest za duzy!\n");
			}	
		}//for
		printf("Koniec Master\n");
	  }
	  else
	  {	
		printf("Slave %d\n", rank);
		rows2Fill = sizeOfMatrix/numprocs;		
		received=0;
		while(received<rows2Fill)
		{
			MPI_Recv(A[rank*rows2Fill+received], sizeOfMatrix, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
			received++;
		}
		MPI_Recv(b, sizeOfMatrix, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
		
		jacobFactorization(A, D, M, sumLU, x, x_old, Mx, N, b,rank*rows2Fill,(rank+1)*rows2Fill);
		for(iter=iterations;iter>0;iter--)
		{
				for(i=rank*rows2Fill;i<(rank+1)*rows2Fill;i++)
				{
				    x[i]= N[i]*b[i];
					
				    for(j=0; j<sizeOfMatrix; j++)
				    {
					Mx[i] = M[i][j]*x_old[j];
					x[i] +=Mx[i];
				    }
				}
				for(i=rank*rows2Fill;i<(rank+1)*rows2Fill;i++)
				{
				    x_old[i] = x[i];
				    printf("Slave x_old[%d] = %f\n", i, x_old[i]);
				}
								
				MPI_Send(x_old , sizeOfMatrix, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				MPI_Recv(x_old, sizeOfMatrix, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
				for(i=rank*rows2Fill;i<(rank+1)*rows2Fill;i++)
				{
				    x_old[i] = x[i];
				    printf("Received x_old[%d] = %f\n", i, x_old[i]);
				}
		}
		printf("Koniec slave\n");
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

    MPI_Finalize();
    
    return 0;
}

