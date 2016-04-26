#pragma once

#include "musiclist.h"

enum class Status {
	Playing,
	Paused,
	Stoped,
	Exit,
};

class Music {
public:
	Music();
	
	Status GetStatus() const;
	void SetStatus(Status s);

	MusicList& GetList();
private:
	Status status;	
	MusicList list;
};