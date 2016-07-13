#include "manager.h"
#include "musiclist.h"

#include <cstdio>
#include <thread>
#include <iostream>
#include <fstream>

//Helper functions

std::ifstream::pos_type filesize(std::string filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

//Copied from the net.
void daemonize() {
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    /*if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }*/

    /* Redirect standard files to /dev/null */
    FILE* f = freopen( "/dev/null", "r", stdin);
    if(!f)
        exit(EXIT_FAILURE);
    f = freopen( "/dev/null", "w", stdout);
    if(!f)
        exit(EXIT_FAILURE);
    f = freopen( "/dev/null", "w", stderr);
    if(!f)
        exit(EXIT_FAILURE);
}


//Public functions

Manager::Manager(int argc, char* argv[]) {
    //Load configuration
    //conf.Load();

    if(argc == 2 && argv[1] == std::string("-d")) {
        daemonize();
    } else {
        std::cout << "If you want to run it as a daemon restart it with \"-d\" flag" << std::endl;
    }

    //Check if we have some pid number
    std::ifstream ipid_file(conf.GetPidFile());
    if(ipid_file.is_open()) {

        std::string pid;
        ipid_file >> pid;

        //Check if that pid is a real process
        std::ifstream f("/proc/"+pid+"/comm");
        if(f.is_open()) {
            std::string comm;
            f >> comm;
            if(comm == "dplayer++")
                //TODO: We should use argv[0] instead
                throw std::runtime_error("Server is already running");
        }
    }
    ipid_file.close();

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
    
    Music music;

    music.GetList().LoadDir(conf.GetDir());
    std::thread mplayer( [&music] { music.PlayList(); } );

    if(conf.GetAutostart()) {
        music.SetStatus(Status::Playing);
    }

    while(music.GetStatus() != Status::Exit) {
        //TODO: Allow to use more than one protocol simultaneously
        #ifdef _NAMED_PIPE
            ProcessCommand(pipe, music);   
        #elif _TCP_SOCKET
            ProcessCommand(tcp, music);
        #else
        #error At least we need one protocol to use
        #endif
    }
    mplayer.join();
}


//Private Functions 

template <typename T>
void Manager::ProcessCommand(T& proto, Music& music) {
    ExecuteCommand(proto.ReadCommand(), proto, music);
}

template <typename T>
void Manager::ExecuteCommand(Command c, T& proto, Music& music) {
    #ifdef DEBUG
    std::cout << "Command:" <<(int)c << std::endl;
    #endif
    switch (c) {
        case Command::QUIT:
            music.SetStatus(Status::Exit);
            break;
        case Command::PLAY:
            music.SetStatus(Status::Playing);
            break;
        case Command::PAUSE:
            if(music.GetStatus() != Status::Playing) {
                music.SetStatus(Status::Playing);
            } else {
                music.SetStatus(Status::Paused);
            }
            break;
        case Command::STOP:
            music.SetStatus(Status::Stoped);
            break;
        case Command::NEXT:
            music.SetStatus(Status::Forwarding);
            break;
        case Command::BACK:
            music.SetStatus(Status::Backing);
            break;
        case Command::SORT_RANDOM:
            music.GetList().Sort(Order::RANDOM);
            break;
        case Command::GET_ARTIST:
            proto << music.GetCurrent().GetArtist();
            break;
        case Command::GET_TITLE:
            proto << music.GetCurrent().GetTitle();
            break;
        case Command::GET_FILE:
            proto << music.GetCurrent().GetFile();
            break;
        case Command::FILTER_ARTIST:
            {
                //Stop reproduction while we are filtering the list
                auto tmp = music.GetStatus();
                music.SetStatus(Status::Stoped);
                music.GetList().FilterArtist(proto.GetLine());
                music.SetStatus(tmp);
            }
            break;
        case Command::ADD_FOLDER:
            music.GetList().LoadDir(proto.GetLine());
            break;
        case Command::ADD_FILE:
            music.GetList().LoadFile(proto.GetLine());
            break;
        case Command::LOAD_PLAYLIST:
            {
                auto tmp = music.GetStatus();
                music.SetStatus(Status::Stoped);

                // For the moment works passing the full path of the PlayList
                music.GetList().LoadPlaylist(proto.GetLine());
                music.SetStatus(tmp);
            }
            break;
        case Command::VOLUME_SET:
            {
                std::string volum;
                proto >> volum;
                #ifdef DEBUG
                std::cout << volum << std::endl;
                #endif
                music.SetVolume(std::atof(volum.c_str()));
            }
            break;
        case Command::VOLUME_GET:
            proto << std::to_string(music.GetVolume());
            break;
        case Command::TIME_GET_REMAINING:
            proto << music.GetRemainingMilliseconds();
            break;
        case Command::SET_OFFSET:
            music.SetPlayingOffset(std::stoi(proto.GetLine()));
            break;
        case Command::FILE_GET:
            {
                std::ifstream file(music.GetCurrent().GetFile());
                if(!file.is_open())
                    throw std::runtime_error("Can not open music file");

                std::size_t size = filesize(music.GetCurrent().GetFile());
                proto <<  size;
                while(size != 0) {
                    proto.Put(file.get());
                    --size;
                }

                //Flush file
                proto << "";
            }
            break;

        //case Command::SAVE_FILE:
        //  break;
    }
}
