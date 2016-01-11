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
	music.play();

	auto offset = 0;
	unique_lock<std::mutex> lk(cv_m);

	//loop until we have to change the song
	//Note: we supose that sf::Music::Playing == sf::mp3::Playing
	while( !isPrevious() && !isNext() && !isStop() && music.getStatus() == sf::Music::Playing) {

		//Calculate how many milliseconds we have to sleep for finish the song
		offset = music.getPlayingOffset().asMilliseconds();
		auto duration = music.getDuration().asMilliseconds() - offset;

		if(duration < 0) {
			cerr << "What?! The total duration is " << music.getDuration().asMilliseconds() 
				 << "and the offset is " << offset << endl;

			duration = 1;
		}

		std::cout << "Sleeping " << duration << " milliseconds" << endl;

		//Wait until we have something to do or until the song finish
		cv.wait_for(lk, chrono::milliseconds(duration) , 
				[this]{return isPrevious()||isPause()||isStop()||isNext();});

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
