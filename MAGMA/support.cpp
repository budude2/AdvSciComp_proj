#include "support.h"
#include <iostream>

using namespace std;

// ------------------------------------------------------------
// Replace with your code to initialize the dA matrix on the GPU device.
// This simply leverages the CPU version above to initialize it to random values,
// and copies the matrix to the GPU.
// 
// m- # rows
// n- # columns
// dA- pointer to array on GPU
// ldda- leading dimension of the array, this is always rows.
// queue- The queue to operate in.
// ------------------------------------------------------------
void dfill_matrix_A_gpu(magma_int_t m, magma_int_t n, double *dA, magma_int_t ldda, magma_queue_t queue)
{
    double *A;
    magma_int_t lda = ldda;
    magma_dmalloc_cpu(&A, n*lda);

    if (A == NULL)
    {
        fprintf( stderr, "malloc failed\n" );
        return;
    }

    cout << "Filling matrix" << endl;
    readFile("A.bin", A);

    cout << "Copying matrix to GPU" << endl;
    magma_dsetmatrix(m, n, A, lda, dA, ldda, queue);
    
    cout << "Freeing CPU memory" << endl;
    magma_free_cpu(A);
}

