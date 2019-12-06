#include <iostream>
#include <armadillo>
#include "support.h"
#include <chrono>

using namespace std;
using namespace arma;
using namespace std::chrono;

//test A-filename, rows, cols, b-filename, rows, imsize, iterations, relax
//0    1           2     3     4           5     6       7           8
int main(int argc, char *argv[])
{
    auto prog_start = high_resolution_clock::now();

    unsigned long rows_a = strtoul(argv[2], nullptr, 10);
    unsigned long cols_a = strtoul(argv[3], nullptr, 10);

    unsigned long rows_b = strtoul(argv[5], nullptr, 10);
    unsigned long cols_b = 1;

    unsigned long im_size = strtoul(argv[6], nullptr, 10);
    unsigned long iterations = strtoul(argv[7], nullptr, 10);
    double relax = stod(argv[8]);

    cout << "---------------Parameters---------------" << endl;
    cout << "A: " << rows_a << "x" << cols_a << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;
    cout << "Image size: " << im_size << "x" << im_size << endl;
    cout << "Iterations: " << iterations << endl;
    cout << "Relax: " << relax << endl;
    cout << "----------------------------------------" << endl;

    mat A = zeros<dmat>(rows_a, cols_a);
    mat b = zeros<dmat>(rows_b, cols_b);

    dmat x;
    x.zeros(A.n_cols, iterations + 1);

    dmat x0;
    x0.zeros(im_size * im_size, 1);

    A.load(argv[1]);
    A.reshape(rows_a, cols_a);

    b.load(argv[4]);

    auto setup_finish = high_resolution_clock::now();

    ART(A, b, x0, iterations, relax, x);

    auto ART_finish = high_resolution_clock::now();

    x.save("x.bin", raw_binary);

    auto datawrite_finish = high_resolution_clock::now();

    auto setupTime      = duration_cast<milliseconds>(setup_finish - prog_start);
    auto artTime        = duration_cast<milliseconds>(ART_finish - setup_finish);
    auto datawriteTime  = duration_cast<microseconds>(datawrite_finish - ART_finish);
    auto totalTime      = duration_cast<milliseconds>(datawrite_finish - prog_start);

    cout << "Setup time:      " << setupTime.count()        << " mS" << endl;
    cout << "ART time:        " << artTime.count()          << " mS" << endl;
    cout << "Data Write time: " << datawriteTime.count()    << " uS" << endl;
    cout << "Total time:      " << totalTime.count()        << " mS" << endl;
    cout << endl;

	return 0;
}
