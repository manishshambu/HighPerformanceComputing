# Simple makefile for

CXX = g++
CXXFLAGS=-fopenmp
OBJS=

all: quicksort.exe histogram.exe generateElements.exe

quicksort.exe: quicksort.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o quicksort.exe quicksort.o

quicksort.o: quicksort.cc quicksort.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o quicksort.o quicksort.cc

histogram.exe: histogram.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o histogram.exe histogram.o

histogram.o: histogram.cc histogram.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o histogram.o histogram.cc

generateElements.exe: generateElements.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o generateElements.exe generateElements.o

generateElements.o: generateElements.cc
	 ${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o generateElements.o generateElements.cc

%.exe: %.o ${OBJS}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} $? ${LDLIBS} -o $@

.SECONDARY:
clean:
	-${RM} *.o *.exe *.qs *.histo
