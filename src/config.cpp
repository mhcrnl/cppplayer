#include "config.h"
#include <fstream>
#include <iostream>
#include <mutex>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <spdlog/spdlog.h>


namespace pt = boost::property_tree;

static std::mutex config_mutex;

Config::Config() {
  path dir(CONFIG_FOLDER);
  if(!exists(dir) && !create_directory(dir))
    throw std::runtime_error("Could not create config directory");

  Load();
}

#ifdef _NAMED_PIPE

  std::string Config::GetDaemonPipe() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return opt.daemonpipe;
  }

  std::string Config::GetClientPipe() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return opt.clientpipe;
  }
#endif

#ifdef _TCP_SOCKET
  
  unsigned Config::GetPortNumber() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return opt.portnumber;
  }
  
  std::string Config::GetBindAddress() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return opt.bindaddress;
  }
#endif

std::string Config::GetPidFile() const {
    std::lock_guard<std::mutex> lock(config_mutex);
  return opt.pidfile;
}

std::string Config::GetLogFile() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return opt.logfile;
}

path Config::GetDir() const {
    std::lock_guard<std::mutex> lock(config_mutex);
  return opt.dir;
}

spdlog::level::level_enum Config::GetLogLevel() const {
    std::lock_guard<std::mutex> lock(config_mutex);
    return static_cast<spdlog::level::level_enum>(opt.loglevel);
}

//Private functions

void Config::Load() {
    spdlog::get("global")->info("Using config file {}", CONFIG_FOLDER+"daemon.conf");

  //Try to autodetect music dir
  //TODO: We should use libxdg to parse ~/.config/user-dirs.dirs (if exists)
  //and get the default music dir.
  path default_music(Expand("~/Music/"));
  if(exists(default_music))
    opt.dir = default_music.c_str();


  std::ifstream config(CONFIG_FOLDER+"daemon.conf");
  if(!config.is_open()) {
    spdlog::get("global")->warn("Config file could not be open, using default values");
    //Write a dumb config file
    std::ofstream config(CONFIG_FOLDER+"daemon.conf");
    pt::ptree tree;

    tree.put("pid_file", opt.pidfile);
    tree.put("log_file", opt.logfile);
    tree.put("music_folder", opt.dir);
    tree.put("log_level", opt.loglevel);

    #ifdef _NAMED_PIPE
      tree.put("fifo.daemon_pipe", opt.daemonpipe);
      tree.put("fifo.client_pipe", opt.clientpipe);
    #endif

    #ifdef _TCP_SOCKET
      //TODO: ipv6
      tree.put("tcp.port_number", opt.portnumber);
      tree.put("tcp.bind_address", opt.bindaddress);
    #endif

    pt::write_ini(config, tree);

    #ifdef _NAMED_PIPE
    opt.daemonpipe  =Expand(opt.daemonpipe);
    opt.clientpipe  =Expand(opt.clientpipe);
    #endif
    opt.pidfile     =Expand(opt.pidfile);
    opt.logfile     =Expand(opt.logfile);
    opt.dir         =Expand(opt.dir.c_str());

  } else {
    pt::ptree tree;
    pt::read_ini(config, tree);

    //The second argument of tree.get is the default value
    #ifdef _NAMED_PIPE
    opt.daemonpipe = Expand(tree.get("fifo.daemon_pipe", opt.daemonpipe));
    opt.clientpipe = Expand(tree.get("fifo.client_pipe", opt.clientpipe));
    #endif
    #ifdef _TCP_SOCKET
    opt.portnumber = tree.get("tcp.port_number", opt.portnumber);
    opt.bindaddress = tree.get("tcp.bind_address", opt.bindaddress);
    #endif

    opt.pidfile    = Expand(tree.get("pid_file", opt.pidfile));
    opt.logfile    = Expand(tree.get("log_file", opt.logfile));
    opt.dir        = Expand(tree.get("music_folder", opt.dir).c_str());
    opt.loglevel   = tree.get("log_level", opt.loglevel);
  }
}
