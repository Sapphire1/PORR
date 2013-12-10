
#ifndef CHOLESKYFACTORIZATIONV_H_
#define CHOLESKYFACTORIZATIONV_H_

void cholesky(int size, double** inputMatrix, double** matrixL);

void forwardSolutionPhase(int size, double **factorizedMatrix, double *b, double *result);

void backwardSolutionPhase(int size, double** factorizedMatrix, double* y, double *result);

void matrixTransposition(int size, double** inputMatrix, double **transposed);

double runAlgorithmCholesky(double **inputMatrix, double *b, int size, double *x);


#endif /* CHOLESKYFACTORIZATIONV_H_ */
