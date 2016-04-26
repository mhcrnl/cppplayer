#pragma once

#include <boost/filesystem.hpp>
#include <vector>

using namespace boost::filesystem;

class MusicList {
public:
	//Search music recursively in the "dir" path and add it to the db
	void LoadDir(path dir);

	const std::vector<path>& GetSongList() const;
private:
	bool IsSupported(path p);

	//List of songs paths
	std::vector<path> song_list;
};