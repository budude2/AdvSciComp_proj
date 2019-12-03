#ifndef __FILEH__
#define __FILEH__

    #include<magma_v2.h>

    double readFile(const char *, double *);
    void dfill_matrix_gpu(const char *, magma_int_t, magma_int_t, double *, magma_int_t, magma_queue_t);

#endif

