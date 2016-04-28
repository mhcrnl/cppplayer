#include "musiclist.h"

#include <array>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <random>

//Public functions

void MusicList::LoadDir(path p) {
	if(!is_directory(p)) {
		throw std::runtime_error("Error con el directorio");
	}

	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
		const auto pathSong = entry.path();
		if(is_directory(pathSong))	LoadDir(pathSong);
		
		#ifdef DEBUG
		std::cerr << "Adding " << pathSong << std::endl;
		#endif
		if(IsSupported(pathSong))	song_list.emplace_back(Song(pathSong));
	}
}

void MusicList::Sort(Order s) {
	switch(s) {
		case Order::RANDOM:
			std::random_device rd;
			std::shuffle(song_list.begin(), song_list.end(), std::mt19937(rd()));
			break;
	}
}

const std::vector<Song>& MusicList::GetSongList() const {
	return song_list;
}

//Private functions

bool MusicList::IsSupported(path p) {
	static const std::array<path, 4> formats = {path(".mp3"), path(".flac"), path(".ogg"), path(".wav")};
	for(auto& v : formats)
		if(p.extension() == v)
			return true;

	return false;
}