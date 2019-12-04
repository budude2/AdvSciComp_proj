CC = g++
CFLAGS = -ggdb -Wall
SRCS = main.cpp readFile.cpp support.cpp
PROG = test

ARMADILLO = -larmadillo
MAGMA_LIBS := -L/usr/local/magma/lib/ -lmagma_sparse -lmagma -lcublas -lcudart -lcusparse -lopenblas
LIBS = $(MAGMA_LIBS)

$(PROG):$(SRCS)
			$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm test
