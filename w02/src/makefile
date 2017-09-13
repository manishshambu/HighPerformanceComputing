# Simple makefile for

CXXFLAGS=-fopenmp
OBJS=

all: omp-demo.exe

%.exe: %.o ${OBJS}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} $? ${LDLIBS} -o $@

.SECONDARY:
clean:
	-${RM} *.o *.exe
