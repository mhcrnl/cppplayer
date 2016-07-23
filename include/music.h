#pragma once

#include "musiclist.h"
#include "sound.h"
#include "song.h"

#include <boost/filesystem.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace boost::filesystem;

//Possible states
enum class Status {
	Playing,
	Paused,
	Stoped,
	Exit,

	Forwarding, 
	Backing,
	Restart,
};

//This class implements the basic music class, that is, without playing anything
class BaseMusic {
	public:
	virtual void PlayList();

	virtual Status GetStatus() const;
	virtual void SetStatus(Status s);

	virtual MusicList& GetList();
	virtual Song& GetCurrent();


	//XXX
	virtual void SetVolume(float v){}
	virtual float GetVolume(){}

	virtual int GetRemainingMilliseconds(){}

	virtual void SetPlayingOffset(int ms){}

protected:
	virtual void Reproduce();

	virtual bool IsStatus(Status s);
	virtual bool IsNotStatus(Status s);

	Song song;

	std::atomic<Status> status{Status::Stoped};
	MusicList list;

	std::atomic<bool> status_processed {true};
	std::condition_variable cv;
	std::condition_variable status_cv;
};


class Music : public BaseMusic {
public:

	void SetVolume(float v);
	float GetVolume();

	int GetRemainingMilliseconds();

	void SetPlayingOffset(int ms);
private:
	void Reproduce();

	Sound music;
};