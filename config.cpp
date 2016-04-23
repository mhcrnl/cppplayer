#pragma once

#include "config.h"
#include <string>
#include <fstream>

void Config::Load() {
	std::string home = getenv("HOME");
	if(home.empty()) {
		throw runtime_error("HOME env variable not found, exiting");
	}

	std::ifstream file(home+"/.config/player++");
    if(!file.is_open()) {
      std::cerr << "Config file could not be open, using default values";
      return;
    }

    //Load config file

    file.close();
}