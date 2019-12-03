#include <iostream>
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

    magma_int_t rows_a = strtoul(argv[2], nullptr, 10);
    magma_int_t cols_a = strtoul(argv[3], nullptr, 10);

    magma_int_t rows_b = strtoul(argv[5], nullptr, 10);
    magma_int_t cols_b = 1;

    cout << "A: " << rows_a << "x" << cols_a << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;

    // Begin MAGMA stuff
    double *dA = nullptr;
    double *db = nullptr;

    magma_int_t ldda = rows_a; // Might round up to 32 for GPU performance.
    magma_int_t lddb = rows_b;

    magma_queue_t queue = nullptr;

    magma_dmalloc(&dA, ldda * cols_a); // Allocate memory on GPU
    magma_dmalloc(&db, lddb * cols_b);

    if (dA == nullptr || db == nullptr)
    {
         cerr << "malloc failed" << endl;

         magma_queue_destroy(queue);
         magma_free(dA);
         magma_free(db);
         magma_finalize();
         return -1;
    }

    magma_int_t dev = 0;
    magma_queue_create(dev, &queue);

    dfill_matrix_A_gpu(rows_a, cols_a, dA, ldda, queue);
    dfill_matrix_b_gpu(rows_b, cols_b, db, lddb, queue);

    magma_dprint_gpu(rows_a, cols_a, dA, ldda, queue);
    magma_dprint_gpu(rows_b, cols_b, db, lddb, queue);

    //readA(argv[1], A, rows_a, cols_a);
    //readA(argv[4], b, rows_b, cols_b);

    magma_queue_destroy(queue);
    magma_free(dA);
    magma_finalize();
	return 0;
}

