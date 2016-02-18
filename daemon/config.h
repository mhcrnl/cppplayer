#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

enum class Action : char {
	QUIT,
	NEXT,
	PREVIOUS,
	PAUSE,
	RESTART,
	GET_ARTIST,
};

struct Options {
	//Name of the pipe file
	std::string daemonpipe = "/tmp/dplayer++";
	std::string clientpipe = "/tmp/cplayer++";
	path dir = ".";
};

class Config {
	public:
		void Load(std::string);
		~Config();
		Options GetConfig();
	private:
		void Read(options_description& desc);
		std::ifstream file;
};