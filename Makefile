CC = g++
CFLAGS = -g
SRCS = main.cpp readFile.cpp
PROG = test

OPENCV = -lopencv_core -lopencv_imgcodecs -lboost_filesystem -lboost_system -larmadillo
LIBS = $(OPENCV)

$(PROG):$(SRCS)
			$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
