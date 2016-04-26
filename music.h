#pragma once

#include "musiclist.h"

#include <boost/filesystem.hpp>


using namespace boost::filesystem;

//Possible states
enum class Status {
	Playing,
	Paused,
	Stoped,
	Exit,

	Forwarding, 
	Backing,
};

class Music {
public:
	Music();

	void PlayList();

	//Depending on the extension it will use
	//sfe::mp3 or sd::Music
	void Play(path s);

	Status GetStatus() const;
	void SetStatus(Status s);

	MusicList& GetList();
private:
	Status status;	
	MusicList list;

	//sf::Music music;
	//sfe::mp3 mp3music;
};