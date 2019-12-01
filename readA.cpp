#include <iostream>
#include <armadillo>
#include "support.h"

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    unsigned long rows = strtoul(argv[2], nullptr, 10);
    unsigned long cols = strtoul(argv[3], nullptr, 10);
    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols <<endl;

    mat A = zeros<dmat>(rows, cols);

    double * double_values;
    double_values = (double *) malloc(sizeof(double) * (rows * cols));

    readFile(argv[1], double_values);

    // Iterate through the array and convert it to a matrix.
    for(int c = 0; c < cols; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            A(r, c) = double_values[c * rows + r];
        }
    }

    cout << "\n\n\n";
    A.print("A:");
    cout << "\n\n\n";

    cout << "A(2,2): " << A(1, 1) << endl;
    cout << "A(1,2): " << A(0, 1) << endl;

    free(double_values);
	return 0;
}
