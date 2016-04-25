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
		//Pipe used by the daemon to write to client
		std::string daemonpipe = "/tmp/dplayer++";

		//Pipe used by the client to write to daemon
		std::string clientpipe = "/tmp/cplayer++";

		path dir = ".";

		//If true, change automatically Status::Stoped to Status::Playing
		bool autostart = false;
	}opt;
};