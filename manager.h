#pragma once

//The main work is done in this class
//It process the commands from the client
//and controls the execution of the music
class Manager {
public:
	//Start the main loop
	void StartServer();
private:
	Music music;
	MusicList list;
	Database db;
	Config conf;
};