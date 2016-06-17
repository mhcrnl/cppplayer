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
  //TODO: Maybe we should use boost propertytree library
  #ifdef DEBUG
    std::cout << "Using config file " << MakeAbsolute(CONFIG_FOLDER+"daemon.conf") << std::endl;
  #endif


  //Try to autodetect music dir
  //Here we should use libxdg to parse ~/.config/user-dirs.dirs (if exists)
  //and get the default music dir.
  path default_music(MakeAbsolute("~/Music/"));
  if(exists(default_music))
    opt.dir = default_music.c_str();


	std::ifstream config(MakeAbsolute(CONFIG_FOLDER+"daemon.conf"));
  if(!config.is_open()) {
        std::cerr << "Config file could not be open, using default values" << std::endl;
        //Write a dumb config file
        std::ofstream config(MakeAbsolute(CONFIG_FOLDER+"daemon.conf"));
        config.setf(std::ios::boolalpha);
        config  << "daemon_pipe   = "   << GetDaemonPipe()  << std::endl
                << "client_pipe   = "   << GetClientPipe()  << std::endl
                << "music_folder  = "   << GetDir()         << std::endl
                << "auto_start    = "   << GetAutostart()   << std::endl
                << "pid_file      = "   << GetPidFile()     << std::endl
                << "db_file       = "   << GetDbFile()      << std::endl;
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