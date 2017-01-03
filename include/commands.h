#if 0
#This way i can source this file and debug with bash
N=0; 
for i in $(cat /usr/include/cppplayer/commands.h | grep "," | tr -d '\t' | cut -d ',' -f1 | grep -v /)
do 
	export CPPPLAYER_$i=$(printf %cx%x \\ $N)
	N=$(expr $N + 1)
done 
return
#endif

/// @file commands.h
/// @brief Header file to supply the list of available commands
///
/// The purpose of this header is to provide a lightweight and compatible way of communicate 
/// with the daemon without need to parse a file. This header is compatible with C++, C and bash, 
/// that should allow other languages like Go, Python, etc to easily communicate with the daemon.

#pragma once

#ifdef __cplusplus
/// @enum Command 
enum class Command : char {
#else
typedef enum {
#endif
	QUIT, 	///< Close the daemon
	PLAY, 	///< Reproduce the current playlist
	NEXT,	///< Reproduce the next song in the playlist
	BACK,	///< Reproduce the previous song in the playlist
	PAUSE,	///< Stop the reproduction, or restart it if it was previously paused
	STOP,	///< Stop the reproduction and forgets the current playlist position

	SORT_RANDOM,	///< Sort the list 
	SORT_LLF,		///< Sort with Less Listened First
	SORT_MLF,		///< Sort with Mort Listened First

	// Returns the requested metadata
	GET_ARTIST, ///< Returns the artist of the current song
	GET_TITLE,	///< Returns the title of the current song
	GET_FILE,	///< Returns the absolute path to the file of the current song

	FILTER_ARTIST, 	///< Create a playlist with songs that match an artist

	//Operates with playlists
	//TODO: Avoid duplicates
	ADD_FOLDER,	///< Add songs of a folder to the current playlist and the full list of songs.
	ADD_FILE,	///< Add a single song to the current playlist and the full list of songs

	CLEAR_PLAYLIST, ///< Delete all songs from the current list

	SAVE_FILE,		///< Export the current song to a file (playlist)
	SAVE_PLAYLIST, 	///< Export the song list to a file (playlist)
	LOAD_PLAYLIST,	///< Import songs to the current list from a file (playlist)

	VOLUME_SET,		///< Set the volume (values [0,100])
	VOLUME_GET,		///< Return the current volume

	TIME_GET_REMAINING,	///< Returns the remaining time (in milliseconds) to finish the current song

	SET_OFFSET,	///< Set an offset (in seconds)

	FILE_GET, ///< Get the data of the current song. Format is {size of file}{file content}
	FILE_PUT,
#ifdef __cplusplus
};
#else 
} Command;
#endif

