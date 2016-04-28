#include "song.h"

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

Song::Song(path p) {
	extension = p.extension().c_str();
	file = p.c_str();

	TagLib::FileRef f(p.c_str());


	//TODO: Time consuming, we really should cache it
	if(f.isNull()) {
		artist = title = "Unknown";
	} else {
		auto tmp = f.tag()->artist();
		if( tmp == TagLib::String::null) {
			tmp = TagLib::String("Unknown");
		}
		artist = tmp.to8Bit();

		tmp = f.tag()->title();
		if( tmp == TagLib::String::null) {
			tmp = TagLib::String("Unknown");
		}
		title = tmp.to8Bit();
	}

}

std::string Song::GetFile() {
	return file;
}

std::string Song::GetExtension() {
	return extension;
}