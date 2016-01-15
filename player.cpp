#include <vector>
#include <thread>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>


#include "utils.h"
#include "config.h"
#include "player.h"

using namespace std;

void Player::PlayList() {
	auto songList = list.Get();
	for(auto s = songList.begin(); s != songList.end(); ++s) {
		if(song.isStop())	return;
		if(song.isNext())	song.setNext(0);
		song.Play(*s);
		
		if(song.isPrevious()) {
			song.setPrevious(0);
			s-=2;		
		}
	}
}

void Player::chooseAction(char c) {
	if(c == opt.k.quit) {
		if(song.isPause()) {
			song.setPause(false);
			cv.notify_one();
		}
		song.setStop(1); 
		cv.notify_one();
	} else if(c == opt.k.next) {
		song.setNext(1); 
		cv.notify_one();
	} else if(c == opt.k.pause) {
		if(song.isPause()) song.setPause(0); 
		else song.setPause(1);
		cv.notify_one();
	} else if(c == opt.k.previous) {
		song.setPrevious(1);
		cv.notify_one();
	} else if(c == opt.k.quit) {
		song.setStatus(1);
		cv.notify_one();
	}
}

void Player::Initialize(int argc, char* argv[]) {
	try {
		LoadConfig();
	} catch(...) {
		cout << "Using default config" << endl;
	}
	if(argc>1) {
		//CRAP
		if(argc>2 && *argv[2] == 'd') {
			daemon=true;
			daemonize();
		}
		path p(argv[1]);
		if(is_directory(p)) {
			list.LoadFrom(p);
		} else {
			int fd = open(opt.fpipe.c_str(), O_WRONLY);
			write(fd, argv[1], 1);
			close(fd);
			exit(0);
		}
	} else {
		list.LoadFrom(opt.dir);
	}

	list.Randomize();
}

void Player::LoadConfig() 
try {
	string home = getenv("HOME");
	if(home.empty()) 
		throw runtime_error("HOME env variable not found");

	Config conf;
	conf.Load(home+"/.config/player++");

	options_description desc("Options");
	desc.add_options()
	    ("pipe_name", value<string>(&opt.fpipe))
	    ("music_folder", value<path>(&opt.dir))
	    ("Keys.quit", value<char>(&opt.k.quit))
	    ("Keys.next", value<char>(&opt.k.next))
		("Keys.previous", value<char>(&opt.k.previous))
		("Keys.pause", value<char>(&opt.k.pause))
		("Keys.status", value<char>(&opt.k.status))
		("Keys.filter", value<char>(&opt.k.filter))
		("Keys.Filter.artist", value<char>(&opt.k.Filter.artist))
		;
	conf.Read(desc);
} catch(exception& e) {
	cout << e.what() << endl;
	throw e;	
}

char Player::GetPipeChar() {
	int fd = open(opt.fpipe.c_str(), O_RDONLY);
	char c = 0;
	read(fd, &c, 1);
	close(fd);
	return c;
}

void Player::Run(int argc, char* argv[]) {
	Initialize(argc, argv);
	thread mplayer(&Player::PlayList, this);

	if(!daemon) {
		while(!song.isStop()) {
			char c = getch();
			chooseAction(c);
		}
	} else {
		mkfifo(opt.fpipe.c_str(), 0666);
		song.setPause(1);
		while(!song.isStop()) {
			char c = GetPipeChar();
			chooseAction(c);
		}
		unlink(opt.fpipe.c_str());
	}

	mplayer.join();
}