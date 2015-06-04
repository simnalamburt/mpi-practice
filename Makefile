all: main.cc
	mpic++ $^ -o bin

run:
	mpirun -n 4 ./bin
