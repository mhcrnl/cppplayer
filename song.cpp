#include <iostream>
#include <thread>
#include <mutex>
#include "song.h"
#include "utils.h"

condition_variable cv;
mutex cv_m;
mutex song_mutex;


void Song::checkPlay() {
	while(isPause() || mp3music.getStatus() == sfe::mp3::Playing || music.getStatus() == sf::Music::Playing)
		sf::sleep(sf::seconds(0.4f));
	setNext();
	cv.notify_one();
}

template <typename T>
void Song::Reproduce(path song, T& music) {
	if(!music.openFromFile(song.c_str()))
		return;

	std::cout << "Playing: " << song/*.stem()*/.c_str() << std::endl;
	music.play();

	thread t(&Song::checkPlay, this);

	unique_lock<std::mutex> lk(cv_m);
	while(!isNext()&&!isStop()) {
		cv.wait(lk, [this]{return isPause()||isStop()||isNext();});
		if(isPause()) {
			music.pause();
			cv.wait(lk, [this]{return !isPause();});
			music.play();
		}
	}
	music.stop();
	t.join();
}

void Song::Play(path song) {
	if(song.extension() == path(".mp3")) Reproduce(song, mp3music);
	else	Reproduce(song, music);
}

void Song::setStop() {
	lock_guard<mutex> song_guard(song_mutex);
	stop = stop?0:1;
}

void Song::setNext() {
	lock_guard<mutex> song_guard(song_mutex);
	next = next?0:1;

}

void Song::setPause() {
	lock_guard<mutex> song_guard(song_mutex);
	pause = pause?0:1;
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