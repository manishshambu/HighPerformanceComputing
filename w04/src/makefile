# Simple makefile for

CXX = mpic++
CXXFLAGS=
OBJS=

all: systolic-matvec.exe slurp-file.exe systolic-max.exe local-max.exe

systolic-matvec.exe: systolic-matvec.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o systolic-matvec.exe  systolic-matvec.o

systolic-matvec.o: systolic-matvec.cc
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o systolic-matvec.o systolic-matvec.cc

slurp-file.exe: slurp-file.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o slurp-file.exe slurp-file.o

slurp-file.o: slurp-file.cc slurp-file.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o slurp-file.o slurp-file.cc

systolic-max.exe: systolic-max.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o systolic-max.exe systolic-max.o

systolic-max.o: systolic-max.cc
	 ${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o systolic-max.o systolic-max.cc

local-max.exe: local-max.o
	$(CXX) ${CPPFLAGS} ${CXXFLAGS} -o local-max.exe local-max.o

local-max.o: local-max.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o local-max.o local-max.cc

%.exe: %.o ${OBJS}
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} $? ${LDLIBS} -o $@

.SECONDARY:
clean:
	-${RM} *.o *.exe *.qs *.histo
