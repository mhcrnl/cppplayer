#include <iostream>
#include <thread>
#include <chrono>

#include "music.h"
#include "utils.h"

condition_variable cv;
mutex cv_m;
mutex song_mutex;

template <typename T>
void Music::Reproduce(T& music) {
	if(!music.openFromFile(song))
		return;
	
	
	#ifdef DEBUG
	cout << "Playing: " << song << endl;
	#endif
	
	auto duration = music.getDuration().asMilliseconds();

	music.play();
	unique_lock<std::mutex> lk(cv_m);

	bool loop = true; 
	//loop only if the command is Pause
	while(loop) {
		loop = false;

		//Calculate how many milliseconds we have to sleep for finish the song
		auto offset = music.getPlayingOffset().asMilliseconds();
		auto sleep_time = duration - offset;

		if(sleep_time < 0) {
			//It should never happen
			throw logic_error("Playing offset is greater than total length");
		}

		#ifdef DEBUG
		cout << "Sleeping " << sleep_time << " milliseconds" << endl;
		#endif

		//Wait until we have something to do or until the song finish
		cv.wait_for(lk, chrono::milliseconds(sleep_time), [this]{return isSomething();});

		if(isPause()) {
			music.pause();
			cv.wait(lk, [this]{return !isPause();});
			music.play();
			loop = true;
		} else if(isRestart()) {
			music.setPlayingOffset(sf::seconds(0));
			setRestart(0);
		}
	}
	music.stop();
}

void Music::Play(const path s) {
	song_mutex.lock();
	song = s.c_str();
	song_mutex.unlock();
	if(s.extension() == path(".mp3")) Reproduce(mp3music);
	else	Reproduce(music);
}

void Music::setStop(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	stop = b;
}

void Music::setNext(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	next = b;
}

void Music::setPrevious(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	previous = b;
}

void Music::setPause(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	pause = b;
}

void Music::setStatus(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	status = b;
}

void Music::setRestart(bool b) {
	lock_guard<mutex> song_guard(song_mutex);
	restart = b;
}

bool Music::isStop() const {
	lock_guard<mutex> song_guard(song_mutex);
	return stop;
}

bool Music::isNext() const {
	lock_guard<mutex> song_guard(song_mutex);
	return next;
}

bool Music::isPrevious() const{
	lock_guard<mutex> song_guard(song_mutex);
	return previous;
}

bool Music::isPause() const {
	lock_guard<mutex> song_guard(song_mutex);
	return pause;
}

bool Music::isStatus() const {
	lock_guard<mutex> song_guard(song_mutex);
	return status;
}

bool Music::isRestart() const {
	lock_guard<mutex> song_guard(song_mutex);
	return restart;
}

TagLib::String Music::getArtist() {
	lock_guard<mutex> song_guard(song_mutex);
	TagLib::FileRef f(song); //data race to fix
	auto artist = f.tag()->artist();
	if(artist == TagLib::String::null)
		return TagLib::String("Unknown");
	else
		return artist;
}

TagLib::String Music::getTitle() {
	lock_guard<mutex> song_guard(song_mutex);
	TagLib::FileRef f(song); //data race to fix
	auto title = f.tag()->title();
	if(title == TagLib::String::null)
		return TagLib::String("Unknown");
	else
		return title;
}


inline bool Music::isSomething() const {
	return isStop()||isNext()||isPrevious()||isPause()||isStatus()||isRestart();
}
