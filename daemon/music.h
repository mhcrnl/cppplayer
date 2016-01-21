#include <condition_variable>
#include <boost/filesystem.hpp>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include "mp3.h"

using namespace boost::filesystem;
using namespace std;

extern condition_variable cv;
extern mutex cv_m;

class Music {
	public:
		Music() : stop(0), next(0), pause(0),  previous(0), play(0), status(0){};
		void Play(path song);
		void setStop(bool);
		void setNext(bool);
		void setPrevious(bool);
		void setPause(bool);
		void setStatus(bool);
		void restart();
		bool isStop() const;
		bool isNext() const;
		bool isPrevious() const;
		bool isPause() const;
		bool isStatus() const;
		TagLib::String getArtist();
	private:
		inline bool isSomething() const;
		template <typename T>
		void Reproduce(T&);
		bool stop;
		bool next;
		bool pause;
		bool previous;
		bool play;
		bool status;
		path song;
		sf::Music music;
		sfe::mp3 mp3music;
};
