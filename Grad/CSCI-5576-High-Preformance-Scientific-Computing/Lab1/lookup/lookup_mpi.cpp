// This is the code for parallel implementation
// of lookup file

#include<mpi.h>     // Required for MPI
#include<stdio.h>

#include "main.h"

void lookupVal(int n, int N, double *x, double *y, 
    double *xval, double *values)
{
  for ( int j = 0 ; j < N ; ++j){
      double xval_single = xval[j];
      for ( int i = 0 ; i < n ; ++i){
      	  if ( xval_single >= x[i] && xval_single <= x[i+1] ){
              values[j] = y[i] + (xval_single - x[i]) 
              * (y[i+1]-y[i]) / (x[i+1]-x[i]);
              break;
          }
      }
  }
}

int main(int argc, char* argv[]) {
    // Generate values for x and y
    int n = 100;
    double x[n], y[n];

    for ( int i = 0 ; i < n ; ++i)
    {
        x[i] = i;
        y[i] = i*i;
    }

    // Begin MPI process
    MPI_Init(&argc, &argv);

    int numPE = 4;
    int myPE;
    MPI_Comm_size(MPI_COMM_WORLD, &numPE);
    MPI_Comm_rank(MPI_COMM_WORLD, &myPE);

    int arraySize = 20;
    int subArraySize = arraySize / numPE;

    double* globalArray = new double[arraySize];
    double* localArray = new double[subArraySize];

    // Only root process initializes the global array
    if (myPE == 0) {
        for (int i = 0; i < arraySize; ++i) {
            globalArray[i] = 1.0 * i;
        }
    }

    // Scatter the data to each processor
    MPI_Scatter(globalArray, subArraySize, MPI_DOUBLE, 
        localArray, subArraySize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform function evaluation on the local sub-array
    double localValues[subArraySize];
    lookupVal(n, subArraySize, x, y, localArray, localValues);

    for (int i = 0; i < subArraySize; ++i) {
        cout << localArray[i] << ": " << localValues[i] << endl;
    }

    // Gather the results to the root processor
    double* globalValues = nullptr;
    if (myPE == 0) {
        globalValues = new double[arraySize];
    }
    MPI_Gather(localValues, subArraySize, MPI_DOUBLE, 
    globalValues, subArraySize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Only the root process prints the result
    if (myPE == 0) {
        cout << "Squared array: ";
        for (int i = 0; i < arraySize; ++i) {
            cout << globalValues[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}

