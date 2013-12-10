#ifndef CHOLESKYFACTORIZATIONOPENMP_H_
#define CHOLESKYFACTORIZATIONOPENMP_H_

void choleskyParallel(int size, double** inputMatrix, double **matrixL);

void forwardSolutionPhaseParallel(int size, double **factorizedMatrix, double *b, double *result);

void backwardSolutionPhaseParallel(int size, double** factorizedMatrix, double* y, double *result);

void matrixTranspositionParallel(int size, double** inputMatrix, double **transposed);

double runAlgorithmCholeskyParallel(double **inputMatrix, double *b, int size, double *x);


#endif /* CHOLESKYFACTORIZATIONOPENMP_H_ */
