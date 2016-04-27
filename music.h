#pragma once

#include "musiclist.h"
#include "mp3.h"

#include <boost/filesystem.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace boost::filesystem;

//Possible states
enum class Status {
	Playing,
	Paused,
	Stoped,
	Exit,

	Forwarding, 
	Backing,
	Restart,
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
	//Recieves sfe::mp3 or sf::Music and the path of the song
	//and controls the detached SFML thread
	template <typename T>
	void Reproduce(T&, std::string song);

	Status status;	
	MusicList list;

	sf::Music music;
	sfe::mp3 mp3music;
};