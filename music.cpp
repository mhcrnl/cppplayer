#include "music.h"

Music::Music() : status(Status::Stoped){

}

void Music::PlayList() {
	auto musicList = list.GetSongList();
	for(auto s = musicList.begin(); s != musicList.end(); ++s) {
		if(status == Status::Exit)	return;
		//if(isNext())	setNext(0);
		Play(*s);
		
		if(status == Status::Backing) {
			s-=2;		
			status = Status::Playing;
		}
	}
}

void Music::Play(path s) {
	//song_mutex.lock();
	//song = s.c_str();
	//song_mutex.unlock();
	//if(s.extension() == path(".mp3")) Reproduce(mp3music);
	//else	Reproduce(music);
}

Status Music::GetStatus() const {
	return status;
}

void Music::SetStatus(Status s) {
	status = s;
}

MusicList& Music::GetList() {
	return list;
}