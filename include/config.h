#pragma once

#include "utils.h"

#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <spdlog/spdlog.h>

using namespace boost::program_options;
using namespace boost::filesystem;

static const std::string CONFIG_FOLDER = Expand("~/.config/player++/");

class Config {
public:
	Config();

	#ifdef _NAMED_PIPE
		std::string GetDaemonPipe() const;
		std::string GetClientPipe() const;
	#endif

	#ifdef _TCP_SOCKET
		unsigned GetPortNumber() const;
		std::string GetBindAddress() const;
	#endif
	
	std::string GetPidFile()	const;
	std::string GetDbFile()		const;
	std::string GetLogFile()	const;
	path GetDir() const;
	bool GetAutostart() const;
	spdlog::level::level_enum GetLogLevel() const;

private:
	void Load();
	
	struct Options {
		#ifdef _NAMED_PIPE
			//Pipe used by the daemon to write to client
			std::string daemonpipe 	= CONFIG_FOLDER+"dplayer++";

			//Pipe used by the client to write to daemon
			std::string clientpipe 	= CONFIG_FOLDER+"cplayer++";
		#endif

		#ifdef _TCP_SOCKET
			//TODO: ipv6
			unsigned portnumber		= 6600;
			std::string bindaddress	= "0.0.0.0";
		#endif

		//File to store the pid number so we can check if the daemon is really running
		std::string pidfile 	= CONFIG_FOLDER+"player++.pid";

		//Playlist folder
		std::string playlistfolder = CONFIG_FOLDER+"playlist/";

		//Log file
		std::string logfile		= CONFIG_FOLDER+"log.txt";

		//Location of the songs
		path dir = ".";

		short loglevel = 0;
	}opt;
};
