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
		
		
		if(IsSupported(pathSong))	full_list.emplace_back(std::make_shared<Song>(pathSong));
	}

	//Copy pointers of full_list so song_list
	//So all the songs are able to be reproduced
	for(auto s : full_list)
		song_list.emplace_back(s);

	#ifdef DEBUG
		std::cerr << "Loaded " << p << std::endl;
	#endif
}

void MusicList::Sort(Order s) {
	switch(s) {
		case Order::RANDOM:
			std::random_device rd;
			std::shuffle(song_list.begin(), song_list.end(), std::mt19937(rd()));
			break;
	}
}

void MusicList::FilterArtist(const std::string artist) {
	song_list.clear();
	for(auto s : full_list ) {
		#ifdef DEBUG
			std::cout << "Analyzing " << s->GetFile() << std::endl;
		#endif
		if(s->GetArtist() == artist)
			song_list.emplace_back(s);
	}
}

const std::vector<std::shared_ptr<Song>>& MusicList::GetSongList() const {
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