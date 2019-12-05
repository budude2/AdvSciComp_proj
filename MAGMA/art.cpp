#include <iostream>
#include <fstream>
#include "support.h"
#include <cublas_v2.h>
#include <magma_v2.h>
#include <magma_lapack.h>

using namespace std;

//test A-filename, rows, cols, b-filename, rows
//0    1           2     3     4           5
int main(int argc, char *argv[])
{
    magma_init();

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

    /********************
    * Print for testing *
    ********************/
    magma_dprint(rows_A, cols_A, A, ldA);
    magma_dprint(rows_b, cols_b, b, ldb);

    /****************
    * Copy A to GPU *
    ****************/
    magma_setmatrix(rows_A, cols_A, sizeof(double), A, ldA, dA, lddA, queue); 

    /***************************
    * Attempt Transpose on GPU *
    ***************************/
    magmablas_dtranspose(rows_A, cols_A, dA, lddA, dAT, lddAT, queue); 
    magma_dprint_gpu(rows_AT, cols_AT, dAT, lddAT, queue);

    /*****************
    * Copy AT to CPU *
    *****************/
    magma_getmatrix(rows_AT, cols_AT, sizeof(double), dAT, lddAT, AT, ldAT, queue);
    magma_dprint(rows_AT, cols_AT, AT, ldAT);

    /******************
    * Free the memory *
    ******************/
    magma_free(A);
    magma_free(AT);
    magma_free_cpu(A);
    magma_free_cpu(AT);
    magma_free_cpu(b);

    return 0;
} 
