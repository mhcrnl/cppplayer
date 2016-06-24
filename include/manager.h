#pragma once

#include "config.h"
#include "music.h"
#include "database.h"
#include "commands.h"
#include "protocol.h"


//The main work is done in this class
//It process the commands from the client
//and controls the execution of the music
class Manager {
public:
	//Create/destroy pipes for the client-server comunication
	//and checks if there are other daemon already running
	Manager();
	~Manager();

	//Start the main loop
	void StartServer();
private:
	template <typename T>
	void ProcessCommand(T& proto);

	template <typename T>
	void ExecuteCommand(Command c, T& proto);
	
	Music music;
	Database db;
	Config conf;
};