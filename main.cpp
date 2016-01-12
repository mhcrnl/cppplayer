#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#include <cstdio>
#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <sys/stat.h>
#include <sys/types.h>


#include "song.h"
#include "utils.h"
#include "config.h"

using namespace boost::filesystem;
using namespace std;


void getSongList(path p, vector<path>& songList) {
	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
		const auto pathSong = entry.path();
		if(is_directory(pathSong))	getSongList(pathSong, songList);
		if(isSupported(pathSong))	songList.push_back(pathSong);
	}
}

void PlayList(Song* song, const vector<path>& songList) {
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

void chooseAction(char c, Song* song) {
	switch(c) {
		case Key::Quit:
			if(song->isPause()) {
				song->setPause(false);
				cv.notify_one();
			}
			song->setStop(1); 
			cv.notify_one();
			break;
		case Key::Next: 
			song->setNext(1); 
			cv.notify_one();
			break;
		case Key::Pause:
			if(song->isPause()) song->setPause(0); 
			else song->setPause(1);
			cv.notify_one();
			break;
		case Key::Previous:
			song->setPrevious(1);
			cv.notify_one();
			break;
		case Key::Status:
			song->setStatus(1);
			cv.notify_one();
	}
}

char getChar(const char* fpipe) {
	int fd = open(fpipe, O_RDONLY);
	char c = 0;
	read(fd, &c, 1);
	close(fd);
	return c;
}

int main(int argc, char* argv[])
{
	const char* fpipe = "/tmp/player++";
	bool daemon = false;
	vector<path> songList;
	if(argc>1) {
		//CRAP
		if(argc>2 && *argv[2] == 'd') {
			daemon=true;
			daemonize(); 
		}
		path p(argv[1]);
		if(is_directory(p)) {
			getSongList(p, songList);
		} else {
			int fd = open(fpipe, O_WRONLY);
			write(fd, argv[1], 1);
			close(fd);
			return 0;
		}
	} else {
		getSongList(dir, songList);
	}

	random_device rd;
	shuffle(songList.begin(), songList.end(), mt19937(rd()));

	Song* song = new Song;
	thread mplayer(PlayList, song, songList);
	if(!daemon) {
		while(!song->isStop()) {
			char c = getch();
			chooseAction(c, song);
		}
	} else {
		mkfifo(fpipe, 0666);
		song->setPause(1);
		while(!song->isStop()) {
			char c = getChar(fpipe);
			chooseAction(c, song);
		}
		unlink(fpipe);
	}

	mplayer.join();
	delete song;
}
