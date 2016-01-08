#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#include <condition_variable>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>


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
		if(song->isStop())
			return;
		if(song->isNext())
			song->setNext();
		song->Play(s);
	}
}

int main(int argc, char* argv[])
{
	bool daemon = false;
	vector<path> songList;
	if(argc>1) {
		//FIXME
		if(argc>2) {daemonize(); daemon=true;}
		path p(argv[1]);
		if(is_directory(p))
			getSongList(p, songList);
	} else {
		getSongList(".", songList);
	}

	random_device rd;
    std::mt19937 g(rd());
	shuffle(songList.begin(), songList.end(), g);

	Song* song = new Song;
	thread mplayer(PlayList, song, songList);
	if(!daemon)
	while(!song->isStop()) {
		char c = getch();
		if(c == 'q' || c == 'Q') {
			cv.notify_one();
			song->setStop();
		} else if(c == 'n' || c == 'N') {
			cv.notify_one();
			song->setNext();
		} else if(c == 'p' || c == 'P') {
			if(song->isPause()) {
				song->setPause();
				cv.notify_one();
			} else {
				cv.notify_one();
				song->setPause();
			}
		}
	}
	mplayer.join();
	delete song;
}
