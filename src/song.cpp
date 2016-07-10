#include "song.h"

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <mutex>

static std::mutex song_mutex;


Song::Song(path p) {
    extension = p.extension().c_str();
    file = p.c_str();
}

std::string Song::GetTitle() {
    std::lock_guard<std::mutex> song_guard(song_mutex);
    if(title == "") {
        TagLib::FileRef f(file.c_str());
        if(f.isNull()) {
            artist = title = "Unknown";
        } else {
            auto tmp = f.tag()->title();
            if( tmp == TagLib::String::null) {
                tmp = TagLib::String("Unknown");
            }
            title = tmp.to8Bit();
        }
    }
    return title;
}

std::string Song::GetArtist() {
    std::lock_guard<std::mutex> song_guard(song_mutex);
    if(artist == "") {
        TagLib::FileRef f(file.c_str());
        if(f.isNull()) {
            artist = title = "Unknown";
        } else {
            auto tmp = f.tag()->artist();
            if( tmp == TagLib::String::null) {
                tmp = TagLib::String("Unknown");
            }
            artist = tmp.to8Bit();
            //artist = tmp.toWString();
        }
    }
    return artist;
}

std::string Song::GetFile() {
    std::lock_guard<std::mutex> song_guard(song_mutex);
    return file;
}

std::string Song::GetExtension() {
    std::lock_guard<std::mutex> song_guard(song_mutex);
    return extension;
}