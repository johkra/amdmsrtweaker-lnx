CXX = g++
CXXFLAGS=-Os -Wall -pedantic

all: Info.o AmdMsrTweaker.o WinRing0.o Worker.o
	${CXX} AmdMsrTweaker.o Info.o WinRing0.o Worker.o ${CXXFLAGS} -o amdmsrt

Info.o: Info.cpp Info.h
	${CXX} -c Info.cpp ${CXXFLAGS} -o Info.o

AmdMsrTweaker.o: AmdMsrTweaker.cpp Worker.h Info.h WinRing0.h
	${CXX} -c AmdMsrTweaker.cpp ${CXXFLAGS} -o AmdMsrTweaker.o

WinRing0.o: WinRing0.cpp WinRing0.h StringUtils.h
	${CXX} -c WinRing0.cpp ${CXXFLAGS} -o WinRing0.o

Worker.o: Worker.cpp Worker.h StringUtils.h WinRing0.h
	${CXX} -c Worker.cpp ${CXXFLAGS} -o Worker.o

clean:
	rm -f *.o amdmsrt
