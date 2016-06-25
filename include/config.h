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
	void Load();
	std::string GetDaemonPipe() const;
	std::string GetClientPipe() const;
	std::string GetPidFile()	const;
	std::string GetDbFile()		const;
	path GetDir() const;
	bool GetAutostart() const;
private:
	std::string Expand(const std::string file);
	std::string GetHome();
	struct Options {
		#ifdef _NAMED_PIPE
		//Pipe used by the daemon to write to client
		std::string daemonpipe 	= CONFIG_FOLDER+"dplayer++";

		//Pipe used by the client to write to daemon
		std::string clientpipe 	= CONFIG_FOLDER+"cplayer++";
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