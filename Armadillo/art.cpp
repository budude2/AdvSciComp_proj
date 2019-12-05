#include <iostream>
#include <armadillo>
#include <cmath>

using namespace std;
using namespace arma;

int ART(arma::Mat<double>& A, arma::Mat<double>& b, arma::Mat<double>& x0, unsigned long iterations, double relax, arma::Mat<double>& x)
{
    dmat error, xnext;
    //x.zeros(A.n_cols, iterations + 1);
    error.zeros(1, iterations + 1);

    x.col(0) = x0;

    unsigned long iter = 0;
    unsigned long i = 0;
    for(iter = 0; iter < iterations; iter++)
    {
        xnext = x.col(iter); 

        for(i = 0; i < A.n_rows; i++)
        {
            double normAi;
            normAi = pow(norm(A.row(i)), 2);

            if(normAi > 0)
            {
                double residual;
                //cout << i << endl;
                residual = as_scalar(b(i) - A.row(i) * xnext);
                
                xnext = xnext + relax * (residual * trans(A.row(i)) / normAi);
            }
        }

        x.col(iter + 1) = xnext;
        error(iter + 1) = norm(A * x.col(iter + 1) - b);
    }

    //img = x.col(iter);
    return 0;
}
