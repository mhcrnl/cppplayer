CXX=ccache g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -O2 -msse -msse2 -mmmx

LIBS=-pthread -lsfml-system -lsfml-audio -lmpg123 -lboost_system -lboost_filesystem

DEPS = mp3.h song.h utils.h
OBJ = main.o mp3.o song.o utils.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBS)

player++:	$(OBJ)
	$(CXX) -o player++ $(OBJ) $(CXXFLAGS) $(LIBS)

all: clean player++

clean:
	rm -f player++ *.o

install:
	cp player++ /usr/local/bin/

uninstall:
	rm /usr/local/bin/player++
