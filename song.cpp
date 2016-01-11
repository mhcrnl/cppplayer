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
	while( !isNext() && !isStop() && music.getStatus() == sf::Music::Playing) {

		if(song.extension() != path(".mp3")) offset = music.getPlayingOffset().asMilliseconds();
		auto duration = music.getDuration().asMilliseconds() - offset;

		std::cout << "Sleeping " << duration/1000 << " seconds" << endl;

		cv.wait_for(lk, chrono::milliseconds(duration) ,[this]{return isPause()||isStop()||isNext();});

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

bool Song::isPause() const {
	lock_guard<mutex> song_guard(song_mutex);
	return pause;
}