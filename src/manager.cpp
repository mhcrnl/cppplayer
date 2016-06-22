#include "manager.h"
#include "musiclist.h"

#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <thread>
#include <iostream>

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
			f.close();
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

	//Delete pipes, if exist (the program exit abnormaly)
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());

	mkfifo(conf.GetDaemonPipe().c_str(), 0666);
	mkfifo(conf.GetClientPipe().c_str(), 0666);
}

Manager::~Manager() {
	//Delete pipes
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());

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
		std::ifstream f(conf.GetDaemonPipe());
		ExecuteCommand(ReadCommand(f), f);

		f.get();
	}
	mplayer.join();
}


//Private Functions 

Command Manager::ReadCommand(std::ifstream& f) {
	if(!f.is_open()) {
		throw std::runtime_error("Daemon pipe could not be opened");
	}
	auto tmp = static_cast<Command>(f.get());
	return tmp;
}

void Manager::ExecuteCommand(Command c, std::ifstream& dfile) {
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
			if(music.GetStatus() != Status::Playing)
				music.SetStatus(Status::Playing);
			else
				music.SetStatus(Status::Paused);
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
			{
				std::ofstream cfile(conf.GetClientPipe());
				cfile << music.GetCurrent().GetArtist() << std::endl;
			}
			break;
		case Command::GET_TITLE:
			{
				std::ofstream cfile(conf.GetClientPipe());
				cfile << music.GetCurrent().GetTitle() << std::endl;
			}
			break;
		case Command::GET_FILE:
			{
				std::ofstream cfile(conf.GetClientPipe());
				cfile << music.GetCurrent().GetFile() << std::endl;
			}
			break;
		case Command::FILTER_ARTIST:
			{
				std::string s;
				getline(dfile, s);

				//Stop reproduction while we are filtering the list
				auto tmp = music.GetStatus();
				music.SetStatus(Status::Stoped);
				music.GetList().FilterArtist(s);
				music.SetStatus(tmp);
			}
			break;
		case Command::ADD_FOLDER:
			{
				std::string s;
				getline(dfile, s);
				music.GetList().LoadDir(s);
			}
			break;
		case Command::ADD_FILE:
			{
				std::string s;
				getline(dfile, s);
				music.GetList().LoadFile(s);
			}
			break;
		case Command::VOLUME_SET:
			{
				std::string volum;
				dfile >> volum;
				#ifdef DEBUG
				std::cout << volum << std::endl;
				#endif
				music.SetVolume(std::atof(volum.c_str()));
			}
			break;
		case Command::VOLUME_GET:
			{
				std::ofstream cfile(conf.GetClientPipe());
				cfile << std::to_string(music.GetVolume()) << std::endl;
			}
			break;
		case Command::TIME_GET_REMAINING:
			{
				std::ofstream cfile(conf.GetClientPipe());
				cfile << music.GetRemainingMilliseconds() << std::endl;
			}
		//case Command::SAVE_FILE:
		//	break;
	}
}
