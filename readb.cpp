#include <iostream> 
#include <armadillo>

using namespace std; 
using namespace arma;

double readFile(const char *, double *);

int main(int argc, char *argv[])
{
    unsigned long rows = strtoul(argv[2], nullptr, 10);
    unsigned long cols = 1;
    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols <<endl;

    mat b = zeros<dmat>(rows, cols);

    double * double_values;
    double_values = (double *) malloc(sizeof(double) * rows);

    readFile(argv[1], double_values);

    // Iterate through the array and convert it to a matrix.
    for(int r = 0; r < rows; r++)
    {
        b(r, 0) = double_values[r];
    }

    cout << "\n\n\n";
    b.print("b:");
    cout << "\n\n\n";

    cout << "b(1,1): " << b(0, 0) << endl;
    cout << "b(2,1): " << b(1, 0) << endl;
    
    free(double_values);
	return 0; 
}
