#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <unistd.h>
#include <termios.h>
#include <cstring>

#include "song.h"

using namespace boost::filesystem;
using namespace std;

void getSongList(path p, vector<path>& songList) {
	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
		auto pathSong = entry.path();
		if(is_directory(pathSong))	getSongList(pathSong, songList);
		songList.emplace_back(pathSong);
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

char getch() {
        char buf = 0;
        struct termios old;
        memset(&old, 0, sizeof(old));
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
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
	//song.Play gets blocked while the song is playing
	Song* song = new Song;
	thread mplayer(PlayList, song, songList);
	while(1) {
		char c = getch();
		if(c == 'q' || c == 'Q') {
			song->setStop();
			break;
		} else if(c == 'n' || c == 'N') {
			song->setNext();
		}
	}
	mplayer.join();
	delete song;
}
