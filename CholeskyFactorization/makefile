CC = gcc
LIBS = -lm	
OBJECTS = mainCholesky.o auxiliaryFunctions.o choleskyFactorization.o 
OBJECTS_OPEN_MP = mainCholeskyOpenMP.o auxiliaryFunctionsOpenMP.o choleskyFactorizationOpenMP.o
OBJECTS_VEC = mainCholeskyV.o auxiliaryFunctionsV.o choleskyFactorizationV.o
OPEN_MP_FLAGS = -fopenmp
VECTORIZATION_FLAGS = -O2 -msse3 -ftree-vectorize -ftree-vectorizer-verbose=6 
CLICK_FLAGS = -fcilkplus -lcilkrts -o cilk

choleskySeq: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) 

choleskyOpenmp: $(OBJECTS_OPEN_MP)
	$(CC) -o $@ $^ $(LIBS) $(OPEN_MP_FLAGS)

choleskyVec: $(OBJECTS_VEC)
	$(CC) -o $@ $^ $(LIBS) $(VECTORIZATION_FLAGS)

#nie zaimplementowane
choleskyClick: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) $(CLICK_FLAGS)

#Sequential
mainCholesky.o : mainCholesky.c
	$(CC) -c mainCholesky.c

auxiliaryFunctions.o : auxiliaryFunctions.h auxiliaryFunctions.c
	$(CC) -c auxiliaryFunctions.c

choleskyFactorization.o : choleskyFactorization.c
	$(CC) -c choleskyFactorization.c

#OpenMP
choleskyFactorizationOpenMP.o : choleskyFactorizationOpenMP.h choleskyFactorizationOpenMP.c
	$(CC) $(OPEN_MP_FLAGS) -c choleskyFactorizationOpenMP.c 

mainCholeskyOpenMP.o : mainCholeskyOpenMP.c
	$(CC) $(OPEN_MP_FLAGS) -c mainCholeskyOpenMP.c

auxiliaryFunctionsOpenMP.o : auxiliaryFunctionsOpenMP.h auxiliaryFunctionsOpenMP.c
	$(CC) $(OPEN_MP_FLAGS) -c auxiliaryFunctionsOpenMP.c

#Vectorized

mainCholeskyV.o : mainCholeskyV.c
	$(CC) $(VECTORIZATION_FLAGS) -c mainCholeskyV.c

auxiliaryFunctionsV.o : auxiliaryFunctionsV.h auxiliaryFunctionsV.c
	$(CC) $(VECTORIZATION_FLAGS) -c auxiliaryFunctionsV.c

choleskyFactorizationV.o : choleskyFactorizationV.c
	$(CC) $(VECTORIZATION_FLAGS) -c choleskyFactorizationV.c

clean:
	rm -f *.o mainCholesky auxiliaryFunctions choleskyFactorization mainCholeskyOpenMP auxiliaryFunctionsOpenMP	\ choleskyFactorizationOpenMP mainCholeskyV auxiliaryFunctionsV mainCholeskyV auxiliaryFunctionsOpenMP mainCholeskyV *. choleskySeq	\  *. choleskyOpenmp choleskyVec choleskyClick





