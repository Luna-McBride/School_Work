#include <iostream>
using std :: cout;
using std :: endl;

double* lookupVal(int n, int N, double *x, double *y, double *xval, double *values)
{
  for ( int j = 0 ; j < N ; ++j){
      double xval_single = xval[j];
      for ( int i = 0 ; i < n ; ++i){
      	  if ( xval_single >= x[i] && xval_single <= x[i+1] ){
              values[j] = y[i] + (xval_single - x[i]) * (y[i+1]-y[i]) / (x[i+1]-x[i]);
              cout << values[j] << endl;
              break;
          }
      }
  }
  return values;
}

int main(int argc, char *argv[])
{

  int numPE, myPE;

  int n = 100;
  int N = 3;
  double x[n], y[n];

  for ( int i = 0 ; i < n ; ++i)
    {
      x[i] = i;
      y[i] = i*i;
    }

  double xval[N];  

  xval[0] = 14.0;
  xval[1] = 25.5;

  double values[N];
  double* yval = lookupVal(n,N,x,y,xval, values);

  cout << yval << endl;

  for (int i = 0 ; i < N; ++i){
      cout << "For xval = " << xval[i] << " yval = " << yval[i] << endl;
  }
  cout << "For xval = " << xval[0] << " yval = " << yval[0] << endl;

  return 0;
  
}
