#include <iostream>
#include <limits>
#include <cmath>
#include "mpi.h"


// Simple MPI wrapper
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


using namespace std;

int main(int argc, char **argv) {
  const mpi mpi(argc, argv);
  const double PI25DT = 3.141592653589793238462643;

  cout.precision(16);
  cout.setf(ios::fixed, ios::floatfield);

  bool done = false;
  while (!done) {
    long long n;
    if (mpi.rank() == 0) {
      cout << "Enter the number of intervals: (0 quits) " << flush;
      cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    if (n == 0) { break; }

    const double h = 1.0 / double(n);
    double sum = 0.0;
    for (int i = mpi.rank() + 1; i <= n; i += mpi.size()) {
      double x = h*(double(i) - 0.5);
      sum += 4.0/(1.0 + x*x);
    }

    double pi;
    double local = h * sum;
    MPI_Reduce(&local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mpi.rank() == 0) {
      cout <<
        "Approximated pi : " << pi << "\n"
        "Error           : " << fabs(pi - PI25DT) << "\n" <<
        endl;
    }
  }

  return 0;
}
