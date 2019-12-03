CC = g++
CFLAGS = -O3 -march=native --std=c++14
SRCS = main.cpp art.cpp
PROG = test

BOOST = -lboost_filesystem -lboost_system
ARMADILLO = -larmadillo
LIBS = $(ARMADILLO)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
art_mp:
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) -fopenmp -lopenblas
art_nv:
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) -lnvblas -lopenblas

art:
	$(CC) $(CFLAGS) -c art.cpp $(LIBS)

clean:
	rm test 
