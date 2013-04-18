CXX = g++
CXXFLAGS=-Os -Wall -pedantic

all:
	${CXX} AmdMsrTweaker.cpp Info.cpp WinRing0.cpp Worker.cpp ${CXXFLAGS} -o amdmsrt


