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
#include "list.h"
#include "music.h"

using namespace std;

void PlayList(Song* song, List* list) {
	auto songList = list->Get();
	for(auto s = songList.begin(); s != songList.end(); ++s) {
		if(song->isStop())	return;
		if(song->isNext())	song->setNext(0);
		song->Play(*s);
		
		if(song->isPrevious()) {
			song->setPrevious(0);
			s-=2;		
		}
	}
}

void chooseAction(Song& song, Action c) {
	if(c == Action::QUIT) {
		if(song.isPause()) {
			song.setPause(false);
			cv.notify_one();
		}
		song.setStop(1); 
		cv.notify_one();
	} else if(c == Action::NEXT) {
		song.setNext(1); 
		cv.notify_one();
	} else if(c == Action::PAUSE) {
		if(song.isPause()) song.setPause(0); 
		else song.setPause(1);
		cv.notify_one();
	} else if(c == Action::PREVIOUS) {
		song.setPrevious(1);
		cv.notify_one();
	}
}

Action GetPipeChar(Options opt) {
	int fd = open(opt.fpipe.c_str(), O_RDONLY);
	Action c;
	read(fd, &c, 1);
	close(fd);
	return c;
}


int main(int argc, char* argv[]) {
	daemonize();

	Config conf;
	auto opt = conf.GetConfig();

	List* list = new List;

	if(argc>1) {
		path p(argv[1]);
		if(is_directory(p)) {
			list->LoadFrom(p);
		}
	} else {
		list->LoadFrom(opt.dir);
	}

	list->Randomize();

	Song* song = new Song;

	thread mplayer(PlayList, song, list);

	mkfifo(opt.fpipe.c_str(), 0666);
	song->setPause(1);
	while(!song->isStop()) {
		Action c = GetPipeChar(opt);
		chooseAction(*song, c);
	}
	unlink(opt.fpipe.c_str());
	
	mplayer.join();
	delete list;
	delete song;
}