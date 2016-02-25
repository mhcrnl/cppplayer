#include <random>
#include <algorithm>
#include <fstream>
#include <boost/range/iterator_range.hpp>

#include "list.h"
#include "utils.h"

void List::LoadFrom(path p) {
	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
		const auto pathSong = entry.path();
		if(is_directory(pathSong))	LoadFrom(pathSong);
		if(isSupported(pathSong))	songList.emplace_back(pathSong);
	}
}

void List::Randomize() {
	random_device rd;
	shuffle(songList.begin(), songList.end(), mt19937(rd()));
}

const vector<path>& List::Get() const {
	return songList;
}

bool List::SaveCache(string filename) {
	ofstream cache(filename);
	if(!cache.is_open())
		return false;
	
	for(auto f : songList) {
		cache << f.native() << endl;
	}
	cache.close();
	return true;
}

bool List::LoadCache(string filename) {
	ifstream cache(filename);
	if(!cache.is_open())
		return false;
	
	string f;
	while(getline(cache, f)) songList.emplace_back(f);

	cache.close();
	return true;
}