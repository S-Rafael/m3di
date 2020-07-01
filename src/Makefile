# Makefile for the m3di project.

CPP_compiler := g++
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
binary := m3di
libs = -lpthread -ljsoncpp
CPPFLAGS = -std=c++14 -O3 -Wall

# We use the implicit make rules
${binary}: $(objects)
	$(CPP_compiler) -o $(binary) $(libs) $(objects)

clean:
	rm -f *.o $(binary)

all: ${binary}

