
#ifndef CHOLESKYFACTORIZATIONPARALEL_H_
#define CHOLESKYFACTORIZATIONPARALEL_H_

void cholesky(int size, double** inputMatrix, double** matrixL);

void forwardSolutionPhase(int size, double **factorizedMatrix, double *b, double *result);

void backwardSolutionPhase(int size, double** factorizedMatrix, double* y, double *result);

void matrixTransposition(int size, double** inputMatrix, double **transposed);

double runAlgorithmCholesky(double **inputMatrix, double *b, int size);


#endif /* CHOLESKYFACTORIZATIONPARALEL_H_ */
