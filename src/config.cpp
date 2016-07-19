#include "config.h"
#include <fstream>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <spdlog/spdlog.h>


namespace pt = boost::property_tree;

Config::Config() {
  path dir(Expand(CONFIG_FOLDER));
  if(!exists(dir) && !create_directory(dir))
    throw std::runtime_error("Could not create config directory");

  Load();
}

#ifdef _NAMED_PIPE

  std::string Config::GetDaemonPipe() const {
    return opt.daemonpipe;
  }

  std::string Config::GetClientPipe() const {
    return opt.clientpipe;
  }

#elif _TCP_SOCKET
  
  unsigned Config::GetPortNumber() const {
    return opt.portnumber;
  }
  
  std::string Config::GetBindAddress() const {
    return opt.bindaddress;
  }

#else 
#error At least we need one protocol to use
#endif

std::string Config::GetPidFile() const {
  return opt.pidfile;
}

std::string Config::GetDbFile() const {
  return opt.dbfile;
}

std::string Config::GetLogFile() const {
    return opt.logfile;
}

path Config::GetDir() const {
  return opt.dir;
}

bool Config::GetAutostart() const {
  return opt.autostart;
}

//Private functions

void Config::Load() {
  #ifdef DEBUG
    spdlog::get("global")->info("Using config file {}", Expand(CONFIG_FOLDER+"daemon.conf"));
  #endif

  //Try to autodetect music dir
  //TODO: We should use libxdg to parse ~/.config/user-dirs.dirs (if exists)
  //and get the default music dir.
  path default_music(Expand("~/Music/"));
  if(exists(default_music))
    opt.dir = default_music.c_str();


    std::ifstream config(Expand(CONFIG_FOLDER+"daemon.conf"));
  if(!config.is_open()) {
    spdlog::get("global")->warn("Config file could not be open, using default values");
    //Write a dumb config file
    std::ofstream config(Expand(CONFIG_FOLDER+"daemon.conf"));
    pt::ptree tree;

    tree.put("pid_file", opt.pidfile);
    tree.put("db_file", opt.dbfile);
    tree.put("log_file", opt.logfile);
    tree.put("music_folder", opt.dir);
    tree.put("auto_start", opt.autostart);

    #ifdef _NAMED_PIPE
      tree.put("fifo.daemon_pipe", opt.daemonpipe);
      tree.put("fifo.client_pipe", opt.clientpipe);
    #elif _TCP_SOCKET
      //TODO: ipv6
      tree.put("tcp.port_number", opt.portnumber);
      tree.put("tcp.bind_address", opt.bindaddress);
    #else
    #error At least we need one protocol to use
    #endif

    pt::write_ini(config, tree);

    #ifdef _NAMED_PIPE
    opt.daemonpipe  =Expand(opt.daemonpipe);
    opt.clientpipe  =Expand(opt.clientpipe);
    #endif
    opt.pidfile     =Expand(opt.pidfile);
    opt.dbfile      =Expand(opt.dbfile);
    opt.logfile     =Expand(opt.logfile);
    opt.dir         =Expand(opt.dir.c_str());

  } else {
    pt::ptree tree;
    pt::read_ini(config, tree);

    //The second argument of tree.get is the default value
    #ifdef _NAMED_PIPE
    opt.daemonpipe = Expand(tree.get("fifo.daemon_pipe", opt.daemonpipe));
    opt.clientpipe = Expand(tree.get("fifo.client_pipe", opt.clientpipe));
    #elif _TCP_SOCKET
    opt.portnumber = tree.get("tcp.port_number", opt.portnumber);
    opt.bindaddress = tree.get("tcp.bind_address", opt.bindaddress);
    #else
    #error At least we need one protocol to use
    #endif
    opt.pidfile    = Expand(tree.get("pid_file", opt.pidfile));
    opt.dbfile     = Expand(tree.get("db_file", opt.dbfile));
    opt.logfile    = Expand(tree.get("log_file", opt.logfile));
    opt.dir        = Expand(tree.get("music_folder", opt.dir).c_str());
    opt.autostart  = tree.get("auto_start", opt.autostart);
  }
}

std::string Config::Expand(std::string file) {
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