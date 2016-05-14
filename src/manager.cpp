#include "manager.h"
#include "musiclist.h"

#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <thread>

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
	if(!db.Connect(conf.GetDbFile().c_str()))
		throw std::runtime_error("Database could not be opened");
	
	music.GetList().LoadDir(conf.GetDir());
	std::thread mplayer( [this] { music.PlayList(); } );

	if(conf.GetAutostart()) {
		music.SetStatus(Status::Playing);
	}

	while(music.GetStatus() != Status::Exit) {
		ExecuteCommand(ReadCommand());
	}
	mplayer.join();
}


//Private Functions 

Command Manager::ReadCommand() {
	std::ifstream f(conf.GetDaemonPipe());
	if(!f.is_open()) {
		throw std::runtime_error("Daemon pipe could not be opened");
	}

	auto tmp = static_cast<Command>(f.get());
	f.close();
	return tmp;
}

void Manager::ExecuteCommand(Command c) {
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
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetArtist() << std::endl;
				file.close();
			}
			break;
		case Command::GET_TITLE:
			{
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetTitle() << std::endl;
				file.close();
			}
			break;
		case Command::GET_FILE:
			{
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetFile() << std::endl;
				file.close();
			}
			break;
	}
}
