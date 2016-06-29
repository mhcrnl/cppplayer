#pragma once

#include "song.h"


#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <memory>

using namespace boost::filesystem;

enum class Order {
	RANDOM,
};

class MusicList {
public:
	//Search music recursively in the "dir" path and add it to the db
	void LoadDir(path dir);

	void LoadFile(const path pathSong);

	void LoadPlaylist(path pathPl);


	void Sort(Order s);

	void FilterArtist(const std::string artist);

	const std::vector<std::shared_ptr<Song>>& GetSongList() const;
private:
	bool IsSupported(path p);

	//List of songs that will be reproduced
	std::vector<std::shared_ptr<Song>> song_list;

	//List with all songs;
	std::vector<std::shared_ptr<Song>> full_list;
};