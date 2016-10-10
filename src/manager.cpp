#include "manager.h"
#include "musiclist.h"
#include "utils.h"

#include <spdlog/spdlog.h>

#include <cstdio>
#include <thread>
#include <iostream>
#include <fstream>

//Public functions

Manager::Manager(int argc, char* argv[]) {
    //Load configuration
    //conf.Load();
    if(argc == 2 && argv[1] == std::string("-d")) {
        daemonize();
        
        spdlog::drop("global");
        auto logging = spdlog::basic_logger_mt("global", conf.GetLogFile(), true);
        logging->info("Daemonazing server");

    } else {
        std::cout << "If you want to run it as a daemon restart it with \"-d\" flag" << std::endl;
    }
    spdlog::get("global")->set_level(conf.GetLogLevel());

    //Write the pid number
    std::ofstream opid_file(conf.GetPidFile(), std::ios::trunc | std::ios::out);
    if(!opid_file.is_open()) {
        throw std::runtime_error("Pid file could not be opened");
    }

    opid_file << getpid();
    opid_file.close();
}

Manager::~Manager() {
    //Remove pid file
    std::remove(conf.GetPidFile().c_str());
}

void Manager::StartServer() {
    //if(!db.Connect(conf.GetDbFile().c_str()))
    //  throw std::runtime_error("Database could not be opened");
    
    #ifndef _REMOTE_MODE
    Music music;
    #else
    BaseMusic music;
    #endif

    music.GetList().LoadDir(conf.GetDir());
    std::thread mplayer( [&music] { music.PlayList(); } );

    if(conf.GetAutostart()) {
        music.SetStatus(Status::Playing);
    }

    CommandControler cmd(music);


    //FIXME: This should not detach
    #ifdef _NAMED_PIPE
        std::thread([&, this](){
            while(music.GetStatus() != Status::Exit)
                ProcessCommand(pipe, cmd);
        }).detach();
    #endif 

    #ifdef _TCP_SOCKET
        std::thread([&, this](){
            while(music.GetStatus() != Status::Exit)
                ProcessCommand(tcp, cmd);
        }).detach();
    #endif

    mplayer.join();
}

//Private Functions 

template <typename T>
void Manager::ProcessCommand(T& proto, CommandControler& cmd) {
    cmd.Execute(proto.ReadCommand(), proto);
}