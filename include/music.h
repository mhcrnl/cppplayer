#pragma once

#include "musiclist.h"
#include "mp3.h"
#include "song.h"

#include <boost/filesystem.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

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
	void PlayList();

	//Depending on the extension it will use
	//sfe::mp3 or sf::Music
	void Play();

	Status GetStatus() const;
	void SetStatus(Status s);

	void SetVolume(float v);
	float GetVolume();

	int GetRemainingMilliseconds();

	void SetPlayingOffset(int ms);

	MusicList& GetList();
	Song& GetCurrent();
private:
	//Recieves sfe::mp3 or sf::Music and the path of the song
	//and controls the detached SFML thread used to reproduce this song
	template <typename T>
	void Reproduce(T&, const char* song);

	bool IsStatus(Status s);
	bool IsNotStatus(Status s);

	std::atomic<Status> status{Status::Stoped};
	MusicList list;
	Song song;

	sf::Music music;
	sfe::mp3 mp3music;

	std::atomic<bool> status_processed {true};
	std::condition_variable cv;
	std::condition_variable status_cv;

};