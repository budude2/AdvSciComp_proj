#include <iostream>
#include <armadillo>
#include "support.h"

using namespace std;
using namespace arma;

//test A-filename, rows, cols, b-filename, rows
//0    1           2     3     4           5
int main(int argc, char *argv[])
{
    unsigned long rows_a = strtoul(argv[2], nullptr, 10);
    unsigned long cols_a = strtoul(argv[3], nullptr, 10);

    unsigned long rows_b = strtoul(argv[5], nullptr, 10);
    unsigned long cols_b = 1;

    mat A = zeros<dmat>(rows_a, cols_a);
    mat b = zeros<dmat>(rows_b, cols_b);

    mat x = zeros<dmat>(3, 1);

    A.load(argv[1]);
    A.reshape(rows_a, cols_a);

    b.load(argv[4]);

    cout << "\n\n\n";
    A.print("A:");
    cout << "\n\n\n";
    b.print("b:");
    cout << "\n\n\n";

    x = solve(A,b);

    x.print("x:");
    cout << "\n\n\n" << endl;

	return 0;
}
