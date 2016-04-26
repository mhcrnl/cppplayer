#include "config.h"
#include <fstream>
#include <iostream>

void Config::Load() {
	std::string home = getenv("HOME");
	if(home.empty()) {
		throw std::runtime_error("HOME env variable not found, exiting");
	}

	std::ifstream file(home+"/.config/player++");
    if(!file.is_open()) {
      std::cerr << "Config file could not be open, using default values";
      return;
    }

    //Load config file

    file.close();
}

std::string Config::GetDaemonPipe() const {
  return opt.daemonpipe;
}

std::string Config::GetClientPipe() const {
  return opt.clientpipe;
}

path Config::GetDir() const {
  return opt.dir;
}

bool Config::GetAutostart() const {
  return opt.autostart;
}