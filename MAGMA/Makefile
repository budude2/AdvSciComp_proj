CC = g++
CFLAGS = -ggdb -Wall
SRCS = main.cpp readFile.cpp
PROG = test

ARMADILLO = -larmadillo
MAGMA_CFLAGS := $(shell pkg-config --cflags magma)
MAGMA_LIBS := $(shell pkg-config --libs magma)
LIBS = $(MAGMA_LIBS)

$(PROG):$(SRCS)
			$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm test
