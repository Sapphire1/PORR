main:	main.c 
	gcc -o main main.c -lm

vector:	main.c 
	gcc -o vector main.c -lm -O2 -msse3 -ftree-vectorize -ftree-vectorizer-verbose=6 

openmp:	mainOpenMP.c 
	gcc -o openmp mainOpenMP.c  -lm -fopenmp

cilk:	mainCilk.c
	/home/lzmuda/cilkplus-install/bin/gcc -fcilkplus -lcilkrts -o cilk mainCilk.c -lm

