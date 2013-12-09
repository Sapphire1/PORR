#ifndef CHOLESKYFACTORIZATIONPARALLEL_H_
#define CHOLESKYFACTORIZATIONPARALLEL_H_

void choleskyParallel(int size, double** inputMatrix, double **matrixL);

void forwardSolutionPhaseParallel(int size, double **factorizedMatrix, double *b, double *result);

void backwardSolutionPhaseParallel(int size, double** factorizedMatrix, double* y, double *result);

void matrixTranspositionParallel(int size, double** inputMatrix, double **transposed);

double runAlgorithmCholeskyParallel(double **inputMatrix, double *b, int size);

#endif /* CHOLESKYFACTORIZATIONPARALLEL_H_ */
