CXXFLAGS = -fopenmp
CXX ?= icpc

OBJS = histogram.o quicksort.o
TRGS = test_histogram.exe test_quicksort.exe

all: ${TRGS}

%.exe: %.o ${OBJS}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} $^ ${LDLIBS} -o $@

.SECONDARY:
clean:
	-${RM} *.o *.exe
