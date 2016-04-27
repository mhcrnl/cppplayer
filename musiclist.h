#pragma once

#include <boost/filesystem.hpp>
#include <vector>

using namespace boost::filesystem;

enum class Order {
	RANDOM,
};

class MusicList {
public:
	//Search music recursively in the "dir" path and add it to the db
	void LoadDir(path dir);

	void Sort(Order s);

	const std::vector<path>& GetSongList() const;
private:
	bool IsSupported(path p);

	//List of songs paths
	std::vector<path> song_list;
};