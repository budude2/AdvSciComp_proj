#include <iostream>
#include <armadillo>
#include <cmath>

using namespace std;
using namespace arma;

int ART(arma::Mat<double>& AT, arma::Mat<double>& b, arma::Mat<double>& x0, unsigned long iterations, double relax, arma::Mat<double>& x)
{
    dmat error, xnext;
    x.col(0) = x0;

    unsigned long iter = 0;
    unsigned long i = 0;
    for(iter = 0; iter < iterations; iter++)
    {
        xnext = x.col(iter); 

        for(i = 0; i < AT.n_cols; i++)
        {
            double normAi;
            normAi = pow(norm(AT.col(i)), 2);

            if(normAi > 0)
            {
                double residual;
                //cout << i << endl;
                residual = as_scalar(b(i) - trans(AT.col(i)) * xnext);
                
                xnext = xnext + relax * (residual * AT.col(i) / normAi);
            }
        }

        x.col(iter + 1) = xnext;
    }

    return 0;
}
