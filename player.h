#include <string>
#include <boost/filesystem.hpp>

#include "list.h"
#include "song.h"

using namespace boost::filesystem;


struct Key {
	char Quit = 'q';
	char Next = 'n';
	char Previous = 'b';
	char Pause = 'p';
	char Status = 's';
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
		void Run();
		void chooseAction(char c);
	private:
		char GetPipeChar();
		bool daemon;
		List list;
		Song song;
		Options opt;
};