#include "config.h"
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>


using namespace boost::program_options;


void Config::Load() {
	std::string home = getenv("HOME");
	if(home.empty()) {
		throw std::runtime_error("HOME env variable not found, exiting");
	}

	std::ifstream config(home+"/.config/player++");
  if(!config.is_open()) {
    std::cerr << "Config file could not be open, using default values" << std::endl;
    return;
  }

  options_description desc("Options");
  desc.add_options()
      ("daemon_pipe", value<std::string>(&opt.daemonpipe))
      ("client_pipe", value<std::string>(&opt.clientpipe))
      ("pid_file",    value<std::string>(&opt.pidfile))
      ("music_folder", value<path>(&opt.dir))
      ("auto_start", value<bool>(&opt.autostart))
    ;


  variables_map vm = variables_map();
  store(parse_config_file(config , desc, true), vm);
  notify(vm); 
    //Load config file

  config.close();
}

std::string Config::GetDaemonPipe() const {
  return opt.daemonpipe;
}

std::string Config::GetClientPipe() const {
  return opt.clientpipe;
}

std::string Config::GetPidFile() const {
  return opt.pidfile;
}

path Config::GetDir() const {
  return opt.dir;
}

bool Config::GetAutostart() const {
  return opt.autostart;
}