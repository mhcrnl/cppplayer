#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

class Config {
public:
	void Load();
	std::string GetDaemonPipe() const;
	std::string GetClientPipe() const;
private:
	struct Options {
		std::string daemonpipe = "/tmp/dplayer++";
		std::string clientpipe = "/tmp/cplayer++";
		path dir = ".";
		bool autostart = false;
	}opt;
};