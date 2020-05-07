all: m3di

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
binary = m3di
libs = -lpthread -ljsoncpp
CPPFLAGS = -std=c++11 -O3 -Wall -Wno-deprecated-declarations

m3di: $(objects)
	g++ $(compiler_flags) -o $(binary) $(libs) $(objects)

clean:
	rm *.o $(binary)

install:
	@[[ -d "${HOME}/bin" ]] || mkdir "${HOME}/bin"
	@cp -u $(binary) "${HOME}/bin/$(binary)"
	@cp -u massgen "${HOME}/bin/massgen"
	@echo "Installed to ${HOME}/bin"
