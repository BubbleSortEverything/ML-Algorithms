# Example makefile for using the mat libraries
BIN=nn
BIN2=nnoneof

# what you want to name your tar/zip file:
TARNAME=Asn2
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
$(BIN2).cpp\
mat.cpp\
rand.cpp\

HDRS=\
rand.h\
mat.h

OBJS=\
$(BIN).o\
mat.o\
rand.o

OBJS2=\
$(BIN2).o\
mat.o\
rand.o

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)

$(BIN2): $(OBJS2)
	$(CXX)  $(CXXFLAGS) $(OBJS2) $(LIBS) -o $(BIN2)	

clean:
	/bin/rm -f *.o nn nnoneof

tar:
	tar -cvf $(TARNAME).tar makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).tar

zip:
	zip $(TARNAME).zip makefile $(EXAMPLES) $(SRCS) $(HDRS) $(EXTRAS)
	ls -l $(TARNAME).zip
