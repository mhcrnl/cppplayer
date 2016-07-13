#pragma once

#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

const std::string CONFIG_FOLDER = "~/.config/player++/";

class Config {
public:
	Config();

	#ifdef _NAMED_PIPE
	std::string GetDaemonPipe() const;
	std::string GetClientPipe() const;
	#elif _TCP_SOCKET
	unsigned GetPortNumber() const;
	std::string GetBindAddress() const;
	#else 
	#error At least we need one protocol to use
	#endif
	std::string GetPidFile()	const;
	std::string GetDbFile()		const;
	path GetDir() const;
	bool GetAutostart() const;
private:
	void Load();
	
	std::string Expand(const std::string file);
	std::string GetHome();
	struct Options {
		#ifdef _NAMED_PIPE
		//Pipe used by the daemon to write to client
		std::string daemonpipe 	= CONFIG_FOLDER+"dplayer++";

		//Pipe used by the client to write to daemon
		std::string clientpipe 	= CONFIG_FOLDER+"cplayer++";
		#elif _TCP_SOCKET
		//TODO: ipv6
		unsigned portnumber		= 6600;
		std::string bindaddress	= "0.0.0.0";
		#else
		#error At least we need one protocol to use
		#endif

		//File to store the pid number so we can check if the daemon is really running
		std::string pidfile 	= CONFIG_FOLDER+"player++.pid";

		//Database file
		std::string dbfile		= CONFIG_FOLDER+"db.sql";

		//Playlist folder
		std::string playlistfolder = CONFIG_FOLDER+"playlist/";

		//Location of the songs
		path dir = ".";

		//If true, change automatically Status::Stoped to Status::Playing
		bool autostart = false;
	}opt;
};