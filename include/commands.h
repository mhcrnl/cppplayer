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
#ifdef __cplusplus
};
#else 
} Command;
#endif

