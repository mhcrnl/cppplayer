#if 0
#This way i can source this file and debug with bash
N=0; 
for i in $(cat /usr/include/cppplayer/commands.h | grep "," | tr -d '\t' | tr -d ',' | tail -n +2)
do 
	export CPPPLAYER_$i=$(printf %cx%x \\ $N)
	N=$(expr $N + 1)
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

	//Operates with playlists
	//Add songs to the current playlist and the full list of songs.
	//TODO: Avoid duplicates
	ADD_FOLDER,
	ADD_FILE,

	//Delete all songs from the current list
	CLEAR_PLAYLIST,

	//Saves the current song to a playlist
	SAVE_FILE,
	//Saves the current list to a playlist
	SAVE_PLAYLIST,
	//Import songs to the current list from a file
	LOAD_PLAYLIST,

	VOLUME_SET,
	VOLUME_GET,

	TIME_GET_REMAINING,


	SET_OFFSET,

	//Get the data of current song or write 
	//Format is <size of file><file content>
	FILE_GET,
	FILE_PUT,
#ifdef __cplusplus
};
#else 
} Command;
#endif

