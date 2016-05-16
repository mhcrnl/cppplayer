#pragma once

#include "musiclist.h"
#include "mp3.h"
#include "song.h"

#include <boost/filesystem.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <mutex>
#include <condition_variable>

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


class MyMutex {
public:
    inline void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count = 1;
        cv.notify_one();
    }

    inline void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]{return count>0;});

        count--;
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    int count {1};
};

class Music {
public:
	void PlayList();

	//Depending on the extension it will use
	//sfe::mp3 or sf::Music
	void Play();

	Status GetStatus() const;
	void SetStatus(Status s);

	MusicList& GetList();
	Song& GetCurrent();
private:
	//Recieves sfe::mp3 or sf::Music and the path of the song
	//and controls the detached SFML thread used to reproduce this song
	template <typename T>
	void Reproduce(T&, const char* song);

	bool IsStatus(Status s);
	bool IsNotStatus(Status s);


	Status status {Status::Stoped};	
	MusicList list;
	Song song;

	sf::Music music;
	sfe::mp3 mp3music;

	MyMutex mymutex;
	std::condition_variable cv;

};