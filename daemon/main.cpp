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
#include <fstream>


#include "utils.h"
#include "config.h"
#include "list.h"
#include "music.h"

using namespace std;

void PlayList(Music* music, List* list) {
	auto musicList = list->Get();
	for(auto s = musicList.begin(); s != musicList.end(); ++s) {
		if(music->isStop())	return;
		if(music->isNext())	music->setNext(0);
		music->Play(*s);
		
		if(music->isPrevious()) {
			music->setPrevious(0);
			s-=2;		
		}
	}
}

void chooseAction(Music& music, Action c, Options& opt) {
	if(c == Action::QUIT) {
		if(music.isPause()) {
			music.setPause(false);
			cv.notify_one();
		}
		music.setStop(1); 
		cv.notify_one();
	} else if(c == Action::NEXT) {
		music.setNext(1); 
		cv.notify_one();
	} else if(c == Action::PAUSE) {
		if(music.isPause()) music.setPause(0); 
		else music.setPause(1);
		cv.notify_one();
	} else if(c == Action::PREVIOUS) {
		music.setPrevious(1);
		cv.notify_one();
	} else if(c == Action::RESTART) {
		music.setRestart(1);
		cv.notify_one();
	} else if(c == Action::GET_ARTIST) {
		fstream file(opt.clientpipe);
		file << music.getArtist() << endl;
		file.close();
	} else if(c == Action::GET_TITLE) {
		fstream file(opt.clientpipe);
		file << music.getTitle() << endl;
		file.close();
	} else if(c == Action::GET_FILE) {
		fstream file(opt.clientpipe);
		file << music.getFile() << endl;
		file.close();
	}
}

int main(int argc, char* argv[]) {

	Config conf;
	Options opt;
	try {
		opt = conf.GetConfig();
	} catch(...) {
		cout << "Using default values" << endl;
	}

	#ifndef DEBUG
	daemonize();
	#endif
	
	List* list = new List;

	
	#ifdef DEBUG
	cout << "No cache found" << endl;
	#endif
	
	if(argc>1) {
		path p(argv[1]);
		if(is_directory(p)) {
			list->LoadFrom(p);
		}
	} else if(!list->LoadCache(opt.cachefile)) {
		list->LoadFrom(opt.dir);
		list->SaveCache(opt.cachefile);
	}


	list->Randomize();

	Music* music = new Music;

	thread mplayer(PlayList, music, list);

	unlink(opt.daemonpipe.c_str()); //Delete pipes, if exist (the program exit abnormaly)
	unlink(opt.clientpipe.c_str());

	mkfifo(opt.daemonpipe.c_str(), 0666);
	mkfifo(opt.clientpipe.c_str(), 0666);

	if(!opt.autostart)
		music->setPause(1);
	
	while(!music->isStop()) {
		ifstream file(opt.daemonpipe);
		Action c = static_cast<Action>(file.get());
		chooseAction(*music, c, opt);
		file.close();
	}

	unlink(opt.daemonpipe.c_str());
	unlink(opt.clientpipe.c_str());
	
	mplayer.join();
	delete list;
	delete music;
}
