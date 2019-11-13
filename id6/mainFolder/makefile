# Example makefile for using the mat libraries
BIN=id6

# what you want to name your tar/zip file:
TARNAME=OK_Assignment7
CXX=g++

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
tree.cpp\

HDRS=\
rand.h\
mat.h\
tree.h

OBJS=\
$(BIN).o\
mat.o\
rand.o\
tree.o


$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)


clean:
	/bin/rm -f *.o nn

tar:
	tar -cvf $(TARNAME).tar makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).tar

zip:
	zip $(TARNAME).zip makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).zip
