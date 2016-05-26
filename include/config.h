#pragma once

#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

const std::string CONFIG_FOLDER = "~/.config/player++/";

class Config {
public:
	void Load();
	std::string GetDaemonPipe() const;
	std::string GetClientPipe() const;
	std::string GetPidFile()	const;
	std::string GetDbFile()		const;
	path GetDir() const;
	bool GetAutostart() const;
private:
	std::string MakeAbsolute(const std::string file);
	std::string GetHome();
	struct Options {
		//Pipe used by the daemon to write to client
		std::string daemonpipe 	= "/tmp/dplayer++";

		//Pipe used by the client to write to daemon
		std::string clientpipe 	= "/tmp/cplayer++";

		//File to store the pid number so we can check if the daemon is really running
		std::string pidfile 	= "/tmp/player++.pid";

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