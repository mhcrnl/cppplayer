#include "musiclist.h"

#include <array>
#include <boost/range/iterator_range.hpp>

//Public functions

void MusicList::LoadDir(path dir) {
	for(auto& entry : boost::make_iterator_range(directory_iterator(dir), {})) {
		const auto pathSong = entry.path();
		if(is_directory(pathSong))	LoadDir(pathSong);
		if(IsSupported(pathSong))	song_list.emplace_back(pathSong);
	}
}

//Private functions

bool MusicList::IsSupported(path p) {
	static const std::array<path, 4> formats = {path(".mp3"), path(".flac"), path(".ogg"), path(".wav")};
	for(auto& v : formats)
		if(p.extension() == v)
			return true;

	return false;
}