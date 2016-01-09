#include <iostream>
#include <thread>
#include "song.h"
#include "utils.h"

condition_variable cv;
mutex cv_m;

void Song::checkPlay() {
	while(isPause() || mp3music.getStatus() == sfe::mp3::Playing)
		sf::sleep(sf::seconds(0.4f));
	setPlay();
	cv.notify_one();
}

void Song::Play(path song) {
	if(!mp3music.openFromFile(song.c_str()))
		return;

	std::cout << "Playing: " << song.stem().c_str() << std::endl;
	mp3music.play();

	thread t(&Song::checkPlay, this);

	unique_lock<std::mutex> lk(cv_m);
	setPlay();
	while(isPlay()) {
		cv.wait(lk, [this]{return !isPlay()||isPause()||isStop()||isNext();});
		if(isStop() || isNext()) {
			mp3music.stop();
		} else if(isPause()){
			mp3music.pause();
			cv.wait(lk, [this]{return !isPause();});
			mp3music.play();
		}
	}
	t.join();
}

void Song::setStop() {
	stop = stop?0:1;
}

void Song::setNext() {
	next = next?0:1;
}

void Song::setPause() {
	pause = pause?0:1;
}

void Song::setPlay() {
	play = play?0:1;
}

bool Song::isStop() const {
	return stop;
}

bool Song::isNext() const {
	return next;
}

bool Song::isPause() const {
	return pause;
}

bool Song::isPlay() const {
	return play;
}