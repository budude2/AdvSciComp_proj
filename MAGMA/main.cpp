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

    magma_int_t rows_a = strtoul(argv[2], nullptr, 10);
    magma_int_t cols_a = strtoul(argv[3], nullptr, 10);

    magma_int_t rows_b = strtoul(argv[5], nullptr, 10);
    magma_int_t cols_b = 1;

    magma_int_t rows_c = rows_a;
    magma_int_t cols_c = cols_b;

    cout << "A: " << rows_a << "x" << cols_a << endl;
    cout << "b: " << rows_b << "x" << cols_b << endl;

    cout << "c: " << rows_c << "x" << cols_c << endl;

    // Begin MAGMA stuff
    double *dA = nullptr;
    double *db = nullptr;
    double *dc = nullptr;

    magma_int_t ldda = rows_a; // Might round up to 32 for GPU performance.
    magma_int_t lddb = rows_b;
    magma_int_t lddc = rows_c;

    magma_queue_t queue = nullptr;

    magma_dmalloc(&dA, ldda * cols_a); // Allocate memory on GPU
    magma_dmalloc(&db, lddb * cols_b);
    magma_dmalloc(&dc, lddb * cols_c);

    if (dA == nullptr || db == nullptr || dc == nullptr)
    {
         cerr << "malloc failed" << endl;

         magma_queue_destroy(queue);
         magma_free(dA);
         magma_free(db);
         magma_free(dc);
         magma_finalize();
         return -1;
    }

    magma_int_t dev = 0;
    magma_queue_create(dev, &queue);

    dfill_matrix_gpu(argv[1], rows_a, cols_a, dA, ldda, queue);
    dfill_vector_gpu(argv[4], rows_b, db, queue);

    magma_dprint_gpu(rows_a, cols_a, dA, ldda, queue);
    magma_dprint_gpu(rows_b, cols_b, db, lddb, queue);

    magma_dgemv(MagmaNoTrans, rows_a, cols_a, 1, dA, ldda, db, 1, 0, dc, 1, queue);

    magma_dprint_gpu(rows_c, cols_c, dc, lddc, queue);

    writeGPUVectorFile("res.bin", 3, dc, queue);

    magma_queue_destroy(queue);
    magma_free(dA);
    magma_free(db);
    magma_free(dc);
    magma_finalize();
	return 0;
}

