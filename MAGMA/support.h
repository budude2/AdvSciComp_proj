#ifndef __FILEH__
#define __FILEH__

    #include<magma_v2.h>

    double readFile(const char *, double *);
    void dfill_matrix_A_gpu(magma_int_t, magma_int_t, double *, magma_int_t, magma_queue_t);
    void dfill_matrix_b_gpu(magma_int_t, magma_int_t, double *, magma_int_t, magma_queue_t);

#endif

