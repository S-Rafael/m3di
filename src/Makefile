# Static Makefile for the m3di project.
#
# written by Rafael Siejakowski, 2020.
#

# Set variables used by implicit make rules
CXX = g++
CPPFLAGS = -std=c++14 -O3 -Wall
LDFLAGS = 
LDLIBS = -lpthread -ljsoncpp

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
binary = m3di

# We use implicit make rules for compiling
# and an explicit rule for linking.
${binary}: $(objects)
	$(CXX) -o ${binary} $(LDLIBS) $(objects)

clean:
	rm -f *.o $(binary)

install:
	@./install.sh $(binary)

all: ${binary}

