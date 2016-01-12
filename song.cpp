#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "song.h"
#include "utils.h"

condition_variable cv;
mutex cv_m;
mutex song_mutex;

template <typename T>
void Song::Reproduce(path song, T& music) {
	if(!music.openFromFile(song.c_str()))
		return;

	std::cout << "Playing: " << song/*.stem()*/.c_str() << std::endl;
	
	auto offset = 0, duration = music.getDuration().asMilliseconds();
	auto sleep_time = duration;

	music.play();
	unique_lock<std::mutex> lk(cv_m);

	//loop until we have to change the song
	//Note: we supose that sf::Music::Playing == sf::mp3::Playing
	while( !isPrevious() && !isNext() && !isStop() && music.getStatus() == sf::Music::Playing) {


		auto prev_sleep = sleep_time;
		//Calculate how many milliseconds we have to sleep for finish the song
		offset = music.getPlayingOffset().asMilliseconds();
		sleep_time = duration - offset;

		//Workaround
		if(sleep_time > prev_sleep) {
			break;
		}


		if(sleep_time < 0) {
			//It should never happen
			throw logic_error("Playing offset is greater than total length");
		}

		std::cout << "Sleeping " << sleep_time << " milliseconds" << endl;

		//Wait until we have something to do or until the song finish
		cv.wait_for(lk, chrono::milliseconds(sleep_time), [this]{return isSomething();});

		if(isPause()) {
			music.pause();
			cv.wait(lk, [this]{return !isPause();});
			music.play();
		}
	}
	music.stop();
}

void Song::Play(path song) {
	if(song.extension() == path(".mp3")) Reproduce(song, mp3music);
	else	Reproduce(song, music);
}

void Song::setStop(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	stop = b;
}

void Song::setNext(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	next = b;
}

void Song::setPrevious(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	previous = b;
}

void Song::setPause(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	pause = b;
}

void Song::setStatus(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	status = b;
}

bool Song::isStop() const {
	lock_guard<mutex> song_guard(song_mutex);
	return stop;
}

bool Song::isNext() const {
	lock_guard<mutex> song_guard(song_mutex);
	return next;
}

bool Song::isPrevious() const{
	lock_guard<mutex> song_guard(song_mutex);
	return previous;
}

bool Song::isPause() const {
	lock_guard<mutex> song_guard(song_mutex);
	return pause;
}

bool Song::isStatus() const {
	lock_guard<mutex> song_guard(song_mutex);
	return status;
}

inline bool Song::isSomething() const {
	return isStop()||isNext()||isPrevious()||isPause()||isStatus();
}
