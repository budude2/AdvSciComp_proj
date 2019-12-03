#include <iostream>
#include "support.h"
#include <cublas_v2.h>
#include <magma_v2.h>
#include <magma_lapack.h>

using namespace std;

// ------------------------------------------------------------
// Replace with your code to initialize the A matrix.
// This simply initializes it to random values.
// Note that A is stored column-wise, not row-wise.
//
// m   - number of rows,    m >= 0.
// n   - number of columns, n >= 0.
// A   - m-by-n array of size lda*n.
// lda - leading dimension of A, lda >= m.
//
// When lda > m, rows (m, ..., lda-1) below the bottom of the matrix are ignored.
// This is helpful for working with sub-matrices, and for aligning the top
// of columns to memory boundaries (or avoiding such alignment).
// Significantly better memory performance is achieved by having the outer loop
// over columns (j), and the inner loop over rows (i), than the reverse.
void dfill_matrix(
    magma_int_t m, magma_int_t n, double *A, magma_int_t lda )
{
    //#define A(i_, j_) A[ (i_) + (j_)*lda ]

    //magma_int_t i, j;
    //for (j=0; j < n; ++j)
    //{
    //    for (i=0; i < m; ++i)
    //    {
    //        A(i,j) = rand() / ((double) RAND_MAX);
    //    }
    //}

    //#undef A
    
    readFile("A.bin", A);
}

// ------------------------------------------------------------
// Replace with your code to initialize the dA matrix on the GPU device.
// This simply leverages the CPU version above to initialize it to random values,
// and copies the matrix to the GPU.
void dfill_matrix_gpu(
    magma_int_t m, magma_int_t n, double *dA, magma_int_t ldda,
    magma_queue_t queue )
{
    double *A;
    magma_int_t lda = ldda;
    magma_dmalloc_cpu( &A, n*lda );

    if (A == NULL)
    {
        fprintf( stderr, "malloc failed\n" );
        return;
    }

    cout << "Filling matrix" << endl;
    dfill_matrix( m, n, A, lda );
    cout << "Copying matrix to GPU" << endl;
    magma_dsetmatrix( m, n, A, lda, dA, ldda, queue );
    cout << "Freeing CPU memory" << endl;
    magma_free_cpu( A );
}


//test A-filename, rows, cols, b-filename, rows
//0    1           2     3     4           5
int main(int argc, char *argv[])
{
    magma_init();

    magma_int_t rows_a = strtoul(argv[2], nullptr, 10);
    magma_int_t cols_a = strtoul(argv[3], nullptr, 10);

    magma_int_t rows_b = strtoul(argv[5], nullptr, 10);
    magma_int_t cols_b = 1;

    cout << "A: " << rows_a << "x" << cols_a << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;

    // Begin MAGMA stuff
    double *dA = nullptr;

    magma_int_t ldda = rows_a; // Might round up to 32 for GPU performance.
    magma_queue_t queue = nullptr;
    magma_dmalloc(&dA, ldda * cols_a); // Allocate memory on GPU

    if (dA == nullptr)
    {
         cerr << "malloc failed" << endl;
         magma_free(dA);
         magma_finalize();
         return -1;
    }

    magma_int_t dev = 0;
    magma_queue_create(dev, &queue);

    dfill_matrix_gpu(rows_a, cols_a, dA, ldda, queue);

    magma_dprint_gpu(rows_a, cols_a, dA, ldda, queue);

    //readA(argv[1], A, rows_a, cols_a);
    //readA(argv[4], b, rows_b, cols_b);

    magma_queue_destroy(queue);
    magma_free(dA);
    magma_finalize();
	return 0;
}

