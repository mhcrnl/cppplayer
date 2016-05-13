CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -Iinclude

LIBS=-pthread -lsfml-system -lsfml-audio -lmpg123 -lboost_system -lboost_filesystem -lboost_program_options -ltag -lsqlite3

DEPS = include/manager.h include/config.h include/music.h include/musiclist.h include/database.h include/mp3.h include/song.h
OBJ = src/main.o src/manager.o src/config.o src/music.o src/musiclist.o src/database.o src/mp3.o src/song.o

src/%.o: src/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

release: clean
release: CXXFLAGS += -O3 -flto
release: LIBS += -flto
release: player++ 

address: CXXFLAGS += -fsanitize=address
address: LIBS += -fsanitize=address
address: debug

thread: CXXFLAGS += -fsanitize=thread
thread: LIBS += -fsanitize=thread
thread: debug

debug: clean
debug: CXXFLAGS += -ggdb -Og -DDEBUG

debug: player++

player++:	$(OBJ)
	$(CXX) -o dplayer++ $(OBJ) $(LIBS)

clean:
	rm -f dplayer++ src/*.o

install:
	cp dplayer++ /usr/local/bin/

uninstall:
	rm /usr/local/bin/dplayer++

