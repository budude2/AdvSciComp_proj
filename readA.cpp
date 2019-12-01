#include <iostream>
#include <armadillo>
#include "support.h"

using namespace std;
using namespace arma;

int readA(const char * filename, arma::Mat<double>& A, unsigned long rows, unsigned long cols)
{
    double * double_values;
    double_values = (double *) malloc(sizeof(double) * (rows * cols));

    readFile(filename, double_values);

    // Iterate through the array and convert it to a matrix.
    for(int c = 0; c < cols; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            A(r, c) = double_values[c * rows + r];
        }
    }

    free(double_values);
	return 0;
}
