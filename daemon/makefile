CXX=ccache g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic 

LIBS=-pthread -lsfml-system -lsfml-audio -lmpg123 -lboost_system -lboost_filesystem -lboost_program_options -ltag

DEPS = mp3.h music.h utils.h list.h config.h
OBJ = main.o mp3.o music.o utils.o list.o config.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

release: clean
release: CXXFLAGS += -O3 -flto
release: LIBS += -flto
release: player++ 

debug: clean
debug: CXXFLAGS += -ggdb -Og -fsanitize=thread -DDEBUG
debug: LIBS += -fsanitize=thread
debug: player++

player++:	$(OBJ)
	$(CXX) -o dplayer++ $(OBJ) $(LIBS)

clean:
	rm -f dplayer++ *.o

install:
	cp dplayer++ /usr/local/bin/

uninstall:
	rm /usr/local/bin/dplayer++

