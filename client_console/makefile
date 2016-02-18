CXX=ccache g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic 

LIBS=-lboost_system -lboost_filesystem -lboost_program_options

DEPS = ../daemon/config.h
OBJ = main.o ../daemon/config.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

release: clean
release: CXXFLAGS += -O3 -flto
release: LIBS += -flto
release: player++ 

debug: clean
debug: CXXFLAGS += -ggdb -Og -fsanitize=thread
debug: LIBS += -fsanitize=thread
debug: player++

player++:	$(OBJ)
	$(CXX) -o player++ $(OBJ) $(LIBS)

clean:
	rm -f player++ *.o

install:
	cp player++ /usr/local/bin/

uninstall:
	rm /usr/local/bin/player++

