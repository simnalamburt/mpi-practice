#include <iostream>
#include "mpi.h"

class mpi {
  int _rank, _size;
public:
  mpi(int &argc, char **&argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
  }

  ~mpi() {
    MPI_Finalize();
  }

  int rank() const { return _rank; }
  int size() const { return _size; }
};

int main(int argc, char **argv) {
  using namespace std;

  const mpi mpi(argc, argv);
  cout << "Hello, I am " << mpi.rank() << " of " << mpi.size() << endl;
  return 0;
}
