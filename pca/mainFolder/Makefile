# Example makefile for using the mat libraries
BIN=pca

# what you want to name your tar/zip file:
TARNAME=Assignment4
CXX=g++ -std=c++11

CXXFLAGS=-O3 -Wall   # optimize
CXXFLAGS=-g -Wall    # debug
LIBS = -lm

EXAMPLES=

EXTRAS=\
randf.cpp\
randmt.cpp

SRCS=\
$(BIN).cpp\
mat.cpp\
rand.cpp\

HDRS=\
rand.h\
mat.h

OBJS=\
$(BIN).o\
mat.o\
rand.o


$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)


clean:
	/bin/rm -f *.o pca

tar:
	tar -cvf $(TARNAME).tar makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).tar

zip:
	zip $(TARNAME).zip makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).zip