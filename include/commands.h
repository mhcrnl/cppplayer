#if 0
#This way i can source this file and debug with bash
N=0; 
for i in $(cat /usr/include/cppplayer/commands.h | grep "," | tr -d '\t' | tr -d ',' | tail -n +2)
do 
	N=$(expr $N + 1)
	export CPPPLAYER_$i=$(printf %cx%x \\ $N)
done 
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

