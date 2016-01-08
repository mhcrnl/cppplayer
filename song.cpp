#include <iostream>

#include "song.h"
#include "utils.h"

void Song::Play(path song) {
	if(!isSupported(song)) {
		std::cerr << "Incorrect format " << song.extension().c_str() << std::endl;
		return;
	}

	if(!music.openFromFile(song.c_str()))
		return;

	std::cout << "Playing " << song.c_str() << std::endl;
	music.play();
	while(!isStop() && !isNext() && music.getStatus() == sfe::mp3::Playing) {
		if(isPause())	{
			music.pause();
			while(isPause())	sf::sleep(sf::seconds(0.4f));
			music.play();
		}
		sf::sleep(sf::seconds(0.4f));
	}
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

bool Song::isStop() const {
	return stop;
}

bool Song::isNext() const {
	return next;
}

bool Song::isPause() const {
	return pause;
}