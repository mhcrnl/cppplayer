#include <boost/filesystem.hpp>
#include "mp3.h"

using namespace boost::filesystem;

class Song {
	public:
		Song() : stop(0), next(0), pause(0){};
		void Play(path song);
		void setStop();
		void setNext();
		void setPause();
		bool isStop() const;
		bool isNext() const;
		bool isPause() const;
	private:
		bool stop;
		bool next;
		bool pause;
		sfe::mp3 music;
};
