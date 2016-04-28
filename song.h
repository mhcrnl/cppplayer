#pragma once

#include <string>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;


class Song {
public:
	Song(path p);
	std::string GetFile();
	std::string GetExtension();
private:
	std::string extension;
	std::string artist;
	std::string title;
	std::string file;
};