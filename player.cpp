#include <vector>
#include <thread>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include "utils.h"
#include "config.h"
#include "ui.h"
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
	if(c == opt.k.Quit) {
		if(song.isPause()) {
			song.setPause(false);
			cv.notify_one();
		}
		song.setStop(1); 
		cv.notify_one();
	} else if(c == opt.k.Next) {
		song.setNext(1); 
		cv.notify_one();
	} else if(c == opt.k.Pause) {
		if(song.isPause()) song.setPause(0); 
		else song.setPause(1);
		cv.notify_one();
	} else if(c == opt.k.Previous) {
		song.setPrevious(1);
		cv.notify_one();
	} else if(c == opt.k.Quit) {
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
		}
		path p(argv[1]);
		if(is_directory(p)) {
			list.LoadFrom(p);
		} else {
			int fd = open(opt.fpipe.c_str(), O_WRONLY);
			write(fd, argv[1], 1);
			close(fd);
			return;
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

	Config conf(home+"/.config/player++");

	po::options_description desc("Options");
	desc.add_options()
	    ("pipe_name", po::value<string>(&opt.fpipe))
	    ("music_folder", po::value<path>(&opt.dir))
	    ("Keys.quit", po::value<char>(&opt.k.Quit))
	    ("Keys.next", po::value<char>(&opt.k.Next))
		("Keys.previous", po::value<char>(&opt.k.Previous))
		("Keys.pause", po::value<char>(&opt.k.Pause))
		("Keys.status", po::value<char>(&opt.k.Status));
	po::variables_map vm;
	conf.Read(desc, vm);
} catch(exception& e) {
	cout << e.what() << endl;
	throw e;	
}

void Player::Run() {
	thread mplayer(&Player::PlayList, this);

	if(!daemon) {
		UIConsole ui;
		while(!song.isStop()) {
			char c = ui.GetChar();
			chooseAction(c);
		}
	} else {
		UICommand ui;
		song.setPause(1);
		while(!song.isStop()) {
			char c = ui.GetChar();
			chooseAction(c);
		}
	}

	mplayer.join();
}