#pragma once

#include <string>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

class Song {
public:
	Song(path p);
	Song() =default;
	std::wstring GetTitle();
	std::wstring GetArtist();
	std::string GetFile();
	std::string GetExtension();
private:
	std::string extension;
	std::wstring artist;
	std::wstring title;
	std::string file;
};