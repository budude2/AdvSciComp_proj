#include <iostream>
#include <armadillo>
#include "support.h"

using namespace std;
using namespace arma;

int readb(const char * filename, arma::Mat<double>& b, unsigned long rows, unsigned long cols)
{
    double * double_values;
    double_values = (double *) malloc(sizeof(double) * rows);

    readFile(filename, double_values);

    // Iterate through the array and convert it to a matrix.
    for(int r = 0; r < rows; r++)
    {
        b(r, 0) = double_values[r];
    }

    free(double_values);
	return 0;
}
