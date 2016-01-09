#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#include <condition_variable>
#include <cstdio>
#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <sys/stat.h>
#include <sys/types.h>


#include "song.h"
#include "utils.h"

using namespace boost::filesystem;
using namespace std;


void getSongList(path p, vector<path>& songList) {
	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
		auto pathSong = entry.path();
		if(is_directory(pathSong))	getSongList(pathSong, songList);
		if(isSupported(pathSong))	songList.push_back(pathSong);
	}
}

void PlayList(Song* song, const vector<path>& songList) {
	for(auto& s : songList) {
		if(song->isStop())	return;
		if(song->isNext())	song->setNext();
		song->Play(s);
	}
}

void chooseAction(char c, Song* song) {
	if(c == 'q' || c == 'Q') {
		song->setStop();
		cv.notify_one();
	} else if(c == 'n' || c == 'N') {
		song->setNext();
		cv.notify_one();
	} else if(c == 'p' || c == 'P') {
		song->setPause();
		cv.notify_one();
	}
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
			if(daemon)
				mkfifo(fpipe, 0666);
			getSongList(p, songList);
		} else {
			int fd = open(fpipe, O_WRONLY);
			write(fd, argv[1], 1);
			close(fd);
			return 0;
		}
	} else {
		getSongList(".", songList);
	}

	random_device rd;
    std::mt19937 g(rd());
	shuffle(songList.begin(), songList.end(), g);

	Song* song = new Song;
	thread mplayer(PlayList, song, songList);
	if(!daemon) {
		while(!song->isStop()) {
			char c = getch();
			chooseAction(c, song);
		}
	} else {
		song->setPause();
		while(!song->isStop()) {
			int fd = open(fpipe, O_RDONLY);
			char c = 0;
			read(fd, &c, 1);
			close(fd);
			chooseAction(c, song);
		}
	}
	unlink(fpipe);

	mplayer.join();
	delete song;
}
