CC = g++
CFLAGS = -O3 -Wall
SRCS = main.cpp
PROG = test

OPENCV = -lopencv_core -lopencv_imgcodecs -lboost_filesystem -lboost_system
LIBS = $(OPENCV)

$(PROG):$(SRCS)
			$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
