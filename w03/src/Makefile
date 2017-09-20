CXX=g++
CXXFLAGS=-I
OBJS=pc-main.exe
SOURCES=producer.cc consumer.cc pc-main.cc
LIBS=-lpthread
EXECUTABLE=pc

pc:
	${CXX} ${SOURCES} -o ${OBJS} ${LIBS}

clean:
	-${RM} *.o *.exe
