#include <iostream>
#include <fstream>
#include "support.h"
#include <cublas_v2.h>
#include <magma_v2.h>
#include <magma_lapack.h>
#include <math.h>

using namespace std;

//test A-filename, rows, cols, b-filename, rows
//0    1           2     3     4           5
int main(int argc, char *argv[])
{
    magma_init();

    /***************************
    * Set algorithm parameters *
    ***************************/
    magma_int_t n_im = 2;
    magma_int_t iterations = 3;
    double relax = 0.5;

    /****************************************************************
    *   This block loads in the sizes passed, and prints them out   *
    ****************************************************************/
    magma_int_t rows_A = strtoul(argv[2], nullptr, 10);
    magma_int_t cols_A = strtoul(argv[3], nullptr, 10);

    magma_int_t rows_b = strtoul(argv[5], nullptr, 10);
    magma_int_t cols_b = 1;

    cout << "A: " << rows_A << "x" << cols_A << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;

    /******************
    * Setup the queue *
    ******************/
    magma_queue_t queue = nullptr;
    magma_int_t dev = 0;
    magma_queue_create(dev, &queue);

    /****************************
    * Setup the A matrix on CPU *
    ****************************/
    double *A;
    magma_int_t ldA = rows_A;
    magma_dmalloc_cpu(&A, ldA * cols_A);

    readFile(argv[1], A);

    /****************************
    * Setup the A matrix on GPU *
    ****************************/
    double *dA = nullptr;
    magma_int_t lddA = ldA;
    magma_dmalloc(&dA, lddA * cols_A);

    /*****************************
    * Setup the AT matrix on CPU *
    *****************************/
    double *AT;
    magma_int_t rows_AT = cols_A;
    magma_int_t cols_AT = rows_A;

    magma_int_t ldAT = rows_AT;
    magma_dmalloc_cpu(&AT, ldAT * cols_AT);

    /*****************************
    * Setup the AT matrix on GPU *
    *****************************/
    double *dAT;
    magma_int_t lddAT = ldAT;
    magma_dmalloc(&dAT, lddAT * cols_AT);

    /****************************
    * Setup the b matrix on CPU *
    ****************************/
    double *b;
    magma_int_t ldb = rows_b;
    magma_dmalloc_cpu(&b, ldb * cols_b);

    readFile(argv[4], b);

    /*********************
    * Setup image vector *
    *********************/
    magma_int_t rows_x = n_im * n_im;
    magma_int_t cols_x = 1;
    magma_int_t lddx = rows_x;

    double *dx = nullptr;
    magma_dmalloc(&dx, lddx * cols_x);
    magmablas_dlaset(MagmaFull, rows_x, cols_x, 0, 0, dx, lddx, queue);        

    // Dummy x for testing
    //double x[4] = {5, 4, 3, 2};    
    //magma_setmatrix(rows_x, cols_x, sizeof(double), x, lddx, dx, lddx, queue); 
    cout << "Initial x: " << endl;
    magma_dprint_gpu(rows_x, cols_x, dx, lddx, queue);

    /********************
    * Print for testing *
    ********************/
    cout << "\nA: " << endl;
    magma_dprint(rows_A, cols_A, A, ldA);
    cout << "\nb: " << endl;
    magma_dprint(rows_b, cols_b, b, ldb);

    /****************
    * Copy A to GPU *
    ****************/
    magma_setmatrix(rows_A, cols_A, sizeof(double), A, ldA, dA, lddA, queue); 

    /***************************
    * Attempt Transpose on GPU *
    ***************************/
    magmablas_dtranspose(rows_A, cols_A, dA, lddA, dAT, lddAT, queue); 
    //magma_dprint_gpu(rows_AT, cols_AT, dAT, lddAT, queue);

    /*****************
    * Copy AT to CPU *
    *****************/
    magma_getmatrix(rows_AT, cols_AT, sizeof(double), dAT, lddAT, AT, ldAT, queue);
    //magma_dprint(rows_AT, cols_AT, AT, ldAT);

    /****************
    * Main ART loop *
    ****************/
    magma_int_t iter;
    magma_int_t r;

    double residual;
    double dotp;
    double alpha;

    /****************************
    * Allocate row on CPU & GPU *
    ****************************/
    double *row;
    magma_dmalloc_cpu(&row, rows_AT * sizeof(double));
    
    magma_int_t rows_row = 1;
    magma_int_t cols_row = cols_A;
    magma_int_t ldrow = rows_row;      // Leading dim is 1, because only 1 row

    double *drow;
    magma_int_t lddrow = ldrow;
    magma_dmalloc(&drow, lddrow * cols_row);

    for(iter = 0; iter < iterations; iter++)
    {
        for(r = 0; r < rows_A; r++)
        {    
            /***************************************
            * Grab a row and take the 2 norm of it *
            ***************************************/
            memcpy(row, &AT[r * cols_A], cols_A * sizeof(double));
            //magma_dprint(1, cols_row, row, ldrow);
    
            double norm;
            norm = pow(magma_cblas_dnrm2(cols_A, row, 1), 2);
            //cout << "Norm: " << norm << endl;

            if(norm > 0)
            {
                /*************************
                * Calculate the residual *
                *************************/
                //cout << "----------------------------" << endl;
                //cout << "Copying vector" << endl;
                magma_setvector(cols_A, sizeof(double), row, 1, drow, 1, queue);
                //magma_dprint_gpu(rows_row, cols_row, drow, lddrow, queue);

                //cout << "Calculate dot product" << endl;
                dotp = magma_ddot(cols_row, drow, 1, dx, 1, queue);
                //cout << dotp << endl;
            
                residual = b[r] - dotp;
                //cout << "residual: " << residual << endl; 
                //cout << "----------------------------" << endl;

                /**********************
                * Calculate the new x *
                **********************/
                alpha = (relax * residual)/norm;
                cout << "Alpha: " << alpha << endl;
                //magma_dscal(cols_row, alpha, drow, 1, queue);
                magma_daxpy(cols_row, alpha, drow, 1, dx, 1, queue);
            }
        }
    }

    cout << "Final x: " << endl;
    magma_dprint_gpu(rows_x, cols_x, dx, lddx, queue);

    writeGPUVectorFile("res.bin", n_im * n_im, dx, queue);
    
    /******************
    * Free the memory *
    ******************/
    magma_free(dA);
    magma_free(dAT);
    magma_free(dx);
    magma_free(drow);
    magma_free_cpu(row);
    magma_free_cpu(A);
    magma_free_cpu(AT);
    magma_free_cpu(b);

    magma_queue_destroy(queue);

    return 0;
} 
