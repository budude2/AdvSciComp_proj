CC = g++
CFLAGS = -g
SRCS = main.cpp
PROG = test

BOOST = -lboost_filesystem -lboost_system
ARMADILLO = -larmadillo
LIBS = $(ARMADILLO)

$(PROG):$(SRCS)
			$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

readA:
	$(CC) $(CFLAGS) -o readA readA.cpp readFile.cpp $(LIBS)

readb:
	$(CC) $(CFLAGS) -o readb readb.cpp readFile.cpp $(LIBS)

clean:
	rm test readA readb
