#include "musiclist.h"

#include <spdlog/spdlog.h>

#include <array>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <random>
#include <fstream>

//Public functions

void MusicList::LoadDir(path p, path metadata) {
    if(!is_directory(p)) {
        throw std::runtime_error("Error con el directorio");
    }

    for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
        const auto pathSong = entry.path();
        if(is_directory(pathSong))  {

            if(metadata.empty()) {
                std::string meta = pathSong.c_str() + std::string("/.METADATA");
                if(exists(meta)) {
                    LoadDir(pathSong, meta);
                } else {
                    LoadDir(pathSong);
                }
            } else {
                LoadDir(pathSong, metadata);
            }

        }  else {
            LoadFile(pathSong, metadata);
        }
    }
}

void MusicList::LoadFile(const path pathSong, path metadata) {
    if(IsSupported(pathSong) && !is_directory(pathSong)) {

        spdlog::get("global")->trace("Loaded {}", pathSong.c_str());

        auto song = std::make_shared<Song>(pathSong, metadata);
        full_list.emplace_back(song);
        song_list.emplace_back(song);
    }
}

void MusicList::LoadPlaylist(std::string pathPl) {
    std::ifstream pl(pathPl);
    if(pl.is_open()) {
        song_list.clear();

        for(std::string l; std::getline(pl, l);) {
            LoadFile(*(new path(l))); // This is clean safe code ???
        }
    }
    pl.close();
}

void MusicList::Sort(Order s) {
    switch(s) {
        case Order::RANDOM:
            std::random_device rd;
            std::shuffle(song_list.begin(), song_list.end(), std::mt19937(rd()));
            break;
    }
}

void MusicList::FilterArtist(const std::string artist) {
    if(artist != "") {
        song_list.clear();
        for(auto s : full_list ) {
            spdlog::get("global")->trace("Analyzing {}", s->GetFile());
            if(s->GetArtist() == artist)
                song_list.emplace_back(s);
        }
    } else {
        song_list = full_list;
    }
}

std::vector<std::shared_ptr<Song>>& MusicList::GetSongList() {
    return song_list;
}

//Private functions

bool MusicList::IsSupported(path p) {
    static const std::array<path, 4> formats = {path(".mp3"), path(".flac"), path(".ogg"), path(".wav")};
    for(auto& v : formats)
        if(p.extension() == v)
            return true;

    return false;
}
