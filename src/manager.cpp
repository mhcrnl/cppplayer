#include "manager.h"
#include "musiclist.h"

#include <cstdio>
#include <thread>
#include <iostream>
#include <fstream>

//Public functions

Manager::Manager() {
	//Load configuration
	conf.Load();

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
	//	throw std::runtime_error("Database could not be opened");
	
	music.GetList().LoadDir(conf.GetDir());
	std::thread mplayer( [this] { music.PlayList(); } );

	if(conf.GetAutostart()) {
		music.SetStatus(Status::Playing);
	}

	while(music.GetStatus() != Status::Exit) {
		//TODO: Allow to use more than one protocol simultaneously
		#ifdef _NAMED_PIPE
			static NamedPipe pipe(conf);
			ProcessCommand(pipe);	
		#elif _TCP_SOCKET
			static Tcp tcp(conf);
			ProcessCommand(tcp);
		#else
		#error At least we need one protocol to use
		#endif
	}
	mplayer.join();
}


//Private Functions 

template <typename T>
void Manager::ProcessCommand(T& proto) {
	ExecuteCommand(proto.ReadCommand(), proto);
}

template <typename T>
void Manager::ExecuteCommand(Command c, T& proto) {
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

		//case Command::SAVE_FILE:
		//	break;
	}
}
