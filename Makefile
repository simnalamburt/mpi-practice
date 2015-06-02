all: main.c
	mpicc $^ -o bin

run:
	mpirun -n 4 ./bin
