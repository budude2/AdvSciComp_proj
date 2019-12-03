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

void dfill_matrix_gpu(const char * fileName, magma_int_t m, magma_int_t n, double *dmatrix, magma_int_t lddmatrix, magma_queue_t queue)
{
    double *matrix;
    magma_int_t ldmatrix = lddmatrix;
    magma_dmalloc_cpu(&matrix, n*ldmatrix);

    if (matrix == NULL)
    {
        fprintf( stderr, "malloc failed\n" );
        return;
    }

    cout << "Filling matrix" << endl;
    readFile(fileName, matrix);

    cout << "Copying matrix to GPU" << endl;
    magma_dsetmatrix(m, n, matrix, ldmatrix, dmatrix, lddmatrix, queue);
    
    cout << "Freeing CPU memory" << endl;
    magma_free_cpu(matrix);
}

