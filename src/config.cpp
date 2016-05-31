#include "config.h"
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>


using namespace boost::program_options;

Config::Config() {
  path dir(MakeAbsolute(CONFIG_FOLDER));
  if(!exists(dir) && !create_directory(dir))
    throw std::runtime_error("Could not create config directory");

}

void Config::Load() {
  #ifdef DEBUG
    std::cout << "Using config file " << MakeAbsolute(CONFIG_FOLDER+"daemon.conf") << std::endl;
  #endif

	std::ifstream config(MakeAbsolute(CONFIG_FOLDER+"daemon.conf"));
  if(!config.is_open()) {
        std::cerr << "Config file could not be open, using default values" << std::endl;
  } else {
    options_description desc("Options");
    desc.add_options()
      ("daemon_pipe", value<std::string>(&opt.daemonpipe))
      ("client_pipe", value<std::string>(&opt.clientpipe))
      ("pid_file",    value<std::string>(&opt.pidfile))
      ("db_file",     value<std::string>(&opt.dbfile))
      ("music_folder", value<path>(&opt.dir))
      ("auto_start", value<bool>(&opt.autostart))

    ;

    variables_map vm = variables_map();
    store(parse_config_file(config , desc, true), vm);
    notify(vm); 
  }

  opt.daemonpipe  =MakeAbsolute(opt.daemonpipe);
  opt.clientpipe  =MakeAbsolute(opt.clientpipe);
  opt.pidfile     =MakeAbsolute(opt.pidfile);
  opt.dbfile      =MakeAbsolute(opt.dbfile);
  opt.dir         =MakeAbsolute(opt.dir.c_str());
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

std::string Config::GetDbFile() const {
  return opt.dbfile;
}

path Config::GetDir() const {
  return opt.dir;
}

bool Config::GetAutostart() const {
  return opt.autostart;
}

//Private functions

std::string Config::MakeAbsolute(std::string file) {
  auto pos = file.find('~');
  if(pos != std::string::npos) {
    file.erase(pos,1);
    file.insert(pos, GetHome());
  }
  
  return file;
}

std::string Config::GetHome() {
  static std::string home = getenv("HOME");
  if(home.empty()) {
    throw std::runtime_error("HOME env variable not found, exiting");
  }
  return home;
}