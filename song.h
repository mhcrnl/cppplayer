#pragma once

#include <string>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;


class Song {
public:
	Song(path p);
	Song() =default;
	std::string GetTitle();
	std::string GetArtist();
	std::string GetFile();
	std::string GetExtension();
private:
	std::string extension;
	std::string artist;
	std::string title;
	std::string file;
};