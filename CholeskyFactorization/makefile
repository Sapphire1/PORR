CC = gcc
CFLAGS =  -O2 -msse3 -ftree-vectorize -ftree-vectorizer-verbose=6 -fopenmp
LIBS = -lm	
OBJECTS = main.o auxiliaryFunctions.o choleskyFactorization.o choleskyFactorizationParallel.o

out: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS) 

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

auxiliaryFunctions.o : auxiliaryFunctions.h auxiliaryFunctions.c
	$(CC) $(CFLAGS) -c auxiliaryFunctions.c

choleskyFactorization.o : auxiliaryFunctions.h choleskyFactorization.c
	$(CC) $(CFLAGS) -c choleskyFactorization.c

choleskyFactorizationParallel.o : auxiliaryFunctions.h choleskyFactorizationParallel.c
	$(CC) $(CFLAGS) -c choleskyFactorizationParallel.c 

clean:
	rm -f *.o main auxiliaryFunctions choleskyFactorization choleskyFactorizationParallel *. out


