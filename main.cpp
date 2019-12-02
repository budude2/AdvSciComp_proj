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

    cout << "A: " << rows_a << "x" << cols_a << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;

    //unsigned long rows_a, cols_a, rows_b, cols_b;
    //rows_a = 36200;
    //cols_a = 40000;

    //rows_b = 36200;
    //cols_b = 1;

    unsigned long iterations = 3;
    double relax = 0.5;

    mat A = zeros<dmat>(rows_a, cols_a);
    mat b = zeros<dmat>(rows_b, cols_b);

    dmat x;
    x.zeros(A.n_cols, iterations + 1);

    dmat x0;
    x0.zeros(200 * 200, 1);

    A.load(argv[1]);
    A.reshape(rows_a, cols_a);

    b.load(argv[4]);

    //cout << "\n\n\n";
    //A.print("A:");
    //cout << "\n\n\n";
    //b.print("b:");
    //cout << "\n\n\n";

    ART(A, b, x0, iterations, relax, x);

    x.save("x.bin", raw_binary);

	return 0;
}
