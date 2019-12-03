#ifndef __FILEH__
#define __FILEH__

    #include<armadillo>

    double readFile(const char *, double *);
    int readA(const char * filename, arma::Mat<double>& A, unsigned long rows, unsigned long cols);
    int readb(const char * filename, arma::Mat<double>& b, unsigned long rows, unsigned long cols);

#endif

