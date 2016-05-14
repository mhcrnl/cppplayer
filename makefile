CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -Iinclude

SRCDIR=src
INCDIR=include
OBJDIR=obj

SOURCES=$(shell find $(SRCDIR) -name "*.cpp")
INCLUDE=$(shell find $(INCDIR) -name "*.h")

LIBS=-pthread \
	 -lsfml-system \
	 -lsfml-audio \
	 -lmpg123 \
	 -lboost_system \
	 -lboost_filesystem \
	 -lboost_program_options \
	 -ltag -lsqlite3

_OBJ = main.o \
	  manager.o \
	  config.o \
	  music.o \
	  musiclist.o \
	  database.o \
	  mp3.o \
	  song.o

OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDE)
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
	rm -f dplayer++ $(OBJDIR)/*.o *~

install:
	cp dplayer++ /usr/local/bin/

uninstall:
	rm /usr/local/bin/dplayer++
