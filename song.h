#include <condition_variable>
#include <boost/filesystem.hpp>
#include "mp3.h"

using namespace boost::filesystem;
using namespace std;

extern condition_variable cv;
extern mutex cv_m;

class Song {
	public:
		Song() : stop(0), next(0), pause(0), play(0){};
		void Play(path song);
		void setStop();
		void setNext();
		void setPause();
		bool isStop() const;
		bool isNext() const;
		bool isPause() const;
	private:
		void checkPlay();
		template <typename T>
		void Reproduce(path song, T& music);
		bool stop;
		bool next;
		bool pause;
		bool play;
		sf::Music music;
		sfe::mp3 mp3music;
};
