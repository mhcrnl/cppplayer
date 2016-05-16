#pragma once

#include "song.h"


#include <boost/filesystem.hpp>
#include <vector>
#include <string>

using namespace boost::filesystem;

enum class Order {
	RANDOM,
};

class MusicList {
public:
	~MusicList();
	//Search music recursively in the "dir" path and add it to the db
	void LoadDir(path dir);

	void Sort(Order s);

	void FilterArtist(const std::string artist);

	const std::vector<Song*>& GetSongList() const;
private:
	bool IsSupported(path p);

	//List of songs that will be reproduced
	std::vector<Song*> song_list;

	//List with all songs;
	std::vector<Song*> full_list;
};