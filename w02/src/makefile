# Simple makefile for

CXX = g++
CXXFLAGS=-fopenmp
OBJS=

all: quicksort.exe histogram.exe

quicksort.exe: quicksort.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o quicksort.exe quicksort.o

quicksort.o: quicksort.cc quicksort.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o quicksort.o quicksort.cc

histogram.exe: histogram.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o histogram.exe histogram.o

histogram.o: histogram.cc histogram.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o histogram.o histogram.cc

%.exe: %.o ${OBJS}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} $? ${LDLIBS} -o $@

.SECONDARY:
clean:
	-${RM} *.o *.exe
