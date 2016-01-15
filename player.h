#include <string>
#include <boost/filesystem.hpp>

#include "list.h"
#include "song.h"

using namespace boost::filesystem;

struct Key {
	char quit 		= 'q';
	char next 		= 'n';
	char previous 	= 'b';
	char pause 		= 'p';
	char status 	= 's';
	char filter		= 'f';
	struct {
		char artist = 'a';
		char album 	= 'b';
		char genre	= 'g';
	}Filter;
};

struct Options {
	//Name of the pipe file
	std::string fpipe = "/tmp/player++";
	path dir = ".";
	Key k;
};

class Player {
	public:
		Player() : daemon(0){};
		void PlayList();
		void Initialize(int argc, char* argv[]);
		void LoadConfig();
		void Run(int argc, char* argv[]);
		void chooseAction(char c);
	private:
		char GetPipeChar();
		bool daemon;
		List list;
		Song song;
		Options opt;
};