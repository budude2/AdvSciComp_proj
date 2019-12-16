#include <iostream>
#include <fstream>
#include "support.h"
#include <cublas_v2.h>
#include <magma_v2.h>
#include <magma_lapack.h>
#include <math.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

//test AT-filename, A rows, A cols, b-filename, rows, imsize, iterations, relax
//0    1            2       3       4           5     6       7           8
int main(int argc, char *argv[])
{
    auto prog_start = high_resolution_clock::now();
    magma_init();

    /***************************
    * Set algorithm parameters *
    ***************************/
    magma_int_t n_im = strtoul(argv[6], nullptr, 10);
    magma_int_t iterations = strtoul(argv[7], nullptr, 10);
    double relax = stod(argv[8]);

    /****************************************************************
    *   This block loads in the sizes passed, and prints them out   *
    ****************************************************************/
    magma_int_t rows_A = strtoul(argv[2], nullptr, 10);
    magma_int_t cols_A = strtoul(argv[3], nullptr, 10);

    magma_int_t rows_b = strtoul(argv[5], nullptr, 10);
    magma_int_t cols_b = 1;

    cout << "---------------Parameters---------------" << endl;
    cout << "AT: "          << rows_A << "x" << cols_A  << endl;
    cout << "b: "           << rows_b << "x" << cols_b  << endl;
    cout << "Image size: "  << n_im   << "x" << n_im    << endl;
    cout << "Iterations: "  << iterations << endl;
    cout << "Relax: "       << relax      << endl;
    cout << "----------------------------------------" << endl;

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
    magma_dmalloc_pinned(&A, ldA * cols_A);

    readFile(argv[1], A);

    /****************************
    * Setup the b matrix on CPU *
    ****************************/
    double *b;
    magma_int_t ldb = rows_b;
    magma_dmalloc_pinned(&b, ldb * cols_b);

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
    //cout << "Initial x: " << endl;
    //magma_dprint_gpu(rows_x, cols_x, dx, lddx, queue);

    /********************
    * Print for testing *
    ********************/
    //cout << "\nA: " << endl;
    //magma_dprint(rows_A, cols_A, A, ldA);
    //cout << "\nb: " << endl;
    //magma_dprint(rows_b, cols_b, b, ldb);

    auto setup_finish = high_resolution_clock::now();

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
    magma_dmalloc_pinned(&row, cols_A * sizeof(double)); // Was rows_AT
    
    magma_int_t rows_row    = 1;
    magma_int_t cols_row    = cols_A;
    magma_int_t ldrow       = rows_row;      // Leading dim is 1, because only 1 row

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
            memcpy(row, &A[r * cols_A], cols_A * sizeof(double));
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
                //cout << "Alpha: " << alpha << endl;
                //magma_dscal(cols_row, alpha, drow, 1, queue);
                magma_daxpy(cols_row, alpha, drow, 1, dx, 1, queue);
            }
        }
    }

    auto ART_finish = high_resolution_clock::now();

    //cout << "Final x: " << endl;
    //magma_dprint_gpu(rows_x, cols_x, dx, lddx, queue);

    cout << "Writing result..." << endl;
    writeGPUVectorFile("x.bin", n_im * n_im, dx, queue);

    /******************
    * Free the memory *
    ******************/
    magma_free(dx);
    magma_free(drow);
    magma_free_pinned(row);
    magma_free_pinned(A);
    magma_free_pinned(b);

    magma_queue_destroy(queue);
    
    auto dwrite_cleanup = high_resolution_clock::now();

    auto setupTime      = duration_cast<milliseconds>(setup_finish - prog_start);
    auto artTime        = duration_cast<milliseconds>(ART_finish - setup_finish);
    auto cleanupTime    = duration_cast<milliseconds>(dwrite_cleanup - ART_finish);
    auto totalTime      = duration_cast<milliseconds>(dwrite_cleanup - prog_start);

    cout << "----------------------------------------" << endl;
    cout << "Setup time:                " << setupTime.count()      << " mS" << endl;
    cout << "ART time:                  " << artTime.count()        << " mS" << endl;
    cout << "Data Write & Cleanup time: " << cleanupTime.count()    << " mS" << endl;
    cout << "Total time:                " << totalTime.count()      << " mS" << endl;
    cout << "----------------------------------------" << endl;

    return 0;
} 
