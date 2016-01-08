#include <boost/filesystem.hpp>
#include "mp3.h"

using namespace boost::filesystem;

class Song {
	public:
		Song() : stop(0), next(0){};
		void Play(path song);
		void setStop();
		void setNext();
		bool isStop() const;
		bool isNext() const;
	private:
		bool stop;
		bool next;
		sfe::mp3 music;
};
