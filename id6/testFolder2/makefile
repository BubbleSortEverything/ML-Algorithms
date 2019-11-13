# Example makefile for using the mat libraries
BIN=id6

# what you want to name your tar/zip file:
TARNAME=ass7_ChaeunKim
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
tree.cpp\
rand.cpp\

HDRS=\
rand.h\
mat.h\
id6.h\
tree.h

OBJS=\
$(BIN).o\
mat.o\
tree.o\
rand.o

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)

clean:
	/bin/rm -f *.o $(BIN)

tar:
	tar -cvf $(TARNAME).tar makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).tar

zip:
	zip $(TARNAME).zip makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).zip