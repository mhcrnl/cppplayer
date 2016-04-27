#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "music.h"

std::condition_variable cv;
std::mutex cv_m;
std::mutex song_mutex;


//Public functions

Music::Music() : status(Status::Stoped){

}

void Music::PlayList() {
	auto musicList = list.GetSongList();
	for(auto s = musicList.begin(); s != musicList.end(); ++s) {
		if(GetStatus() == Status::Exit)	return;
		//if(isNext())	setNext(0);
		Play(*s);
		
		if(GetStatus() == Status::Backing) {
			s-=2;		
			SetStatus(Status::Playing);
		}
	}
}

void Music::Play(path s) {
	if(s.extension() == path(".mp3")) Reproduce(mp3music, s.c_str());
	else	Reproduce(music, s.c_str());
}

Status Music::GetStatus() const {
	std::lock_guard<std::mutex> song_guard(song_mutex);
	return status;
}

void Music::SetStatus(Status s) {
	std::lock_guard<std::mutex> song_guard(song_mutex);
	status = s;
	cv.notify_one();
}

MusicList& Music::GetList() {
	return list;
}


//Private functions

template <typename T>
void Music::Reproduce(T& music, std::string song) {
	if(!music.openFromFile(song))
		return;
	
	
	#ifdef DEBUG
	std::cout << "Playing: " << song << std::endl;
	#endif
	
	auto duration = music.getDuration().asMilliseconds();

	music.play();
	std::unique_lock<std::mutex> lk(cv_m);

	bool loop = true; 
	//loop only if the command is Pause
	while(loop) {
		loop = false;

		//Calculate how many milliseconds we have to sleep for finish the song
		auto offset = music.getPlayingOffset().asMilliseconds();
		auto sleep_time = duration - offset;

		if(sleep_time < 0) {
			//It should never happen
			throw std::logic_error("Playing offset is greater than total length");
		}

		#ifdef DEBUG
		std::cout << "Sleeping " << sleep_time << " milliseconds" << std::endl;
		#endif

		//Wait until we have something to do or until the song finish
		cv.wait_for(lk, std::chrono::milliseconds(sleep_time), [this]{return GetStatus() != Status::Playing;});

		if(GetStatus() == Status::Paused) {
			music.pause();
			cv.wait(lk, [this]{return GetStatus() != Status::Paused;});
			music.play();
			loop = true;
		} else if(GetStatus() == Status::Restart) {
			music.setPlayingOffset(sf::seconds(0));
			status = Status::Playing;
		}
	}
	music.stop();
}