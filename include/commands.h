#if 0
#This way i can source this file and debug with bash
export CPPPLAYER_QUIT="\x00"
export CPPPLAYER_PLAY="\x01"
export CPPPLAYER_NEXT="\x02"
export CPPPLAYER_BACK="\x03"
export CPPPLAYER_PAUSE="\x04"
export CPPPLAYER_STOP="\x05"
export CPPPLAYER_SORT_RANDOM="\x06"
export CPPPLAYER_GET_ARTIST="\x07"
export CPPPLAYER_GET_TITLE="\x08"
export CPPPLAYER_GET_FILE="\x09"
export CPPPLAYER_FILTER_ARTIST="\x0a"
export CPPPLAYER_ADD_FOLDER="\x0b"
export CPPPLAYER_ADD_FILE="\x0c"
export CPPPLAYER_SAVE_FILE="\x0d"
export CPPPLAYER_SAVE_PLAYLIST="\x0e"
export CPPPLAYER_VOLUME_SET="\x0f"
export CPPPLAYER_VOLUME_GET="\x10"
export CPPPLAYER_TIME_GET_REMAINING="\x10"
return
#endif

#pragma once

#ifdef __cplusplus
enum class Command : char {
#else
typedef enum {
#endif
	QUIT,
	PLAY,
	NEXT,
	BACK,
	PAUSE,
	STOP,
	SORT_RANDOM,

	//Return the requested metadata
	GET_ARTIST,
	GET_TITLE,
	GET_FILE,

	//Create a playlist with songs that match an artist.
	FILTER_ARTIST,

	//Add songs to the current playlist and the full list of songs.
	ADD_FOLDER,
	ADD_FILE,

	//Operates with playlists
	//Saves the current songs3	 to a playlist
	SAVE_FILE,
	//Saves the current list to a playlist
	SAVE_PLAYLIST,

	VOLUME_SET,
	VOLUME_GET,

	TIME_GET_REMAINING,

	// Placed here for keeping consistency in existing clients. Future replacement for proper code reading...
	LOAD_PLAYLIST,

	SET_OFFSET,
#ifdef __cplusplus
};
#else 
} Command;
#endif

