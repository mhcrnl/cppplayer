#include <random>
#include <algorithm>
#include <vector>
#include <thread>
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
		if(isSupported(pathSong))	songList.emplace_back(pathSong);
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
	vector<path> songList;
	if(argc>1) {
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
	while(1) {
		char c = getch();
		if(c == 'q' || c == 'Q') {
			song->setStop();
			break;
		} else if(c == 'n' || c == 'N') {
			song->setNext();
		} else if(c == 'p' || c == 'P') {
			song->setPause();
		}
	}
	mplayer.join();
	delete song;
}
