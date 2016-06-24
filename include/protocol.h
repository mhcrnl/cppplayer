#pragma once

#include <fstream>
#include <ostream>
#include <string>
#include "commands.h"


//This should only be used for debugging/scripting pourpose
// or if only one client are in use.
class NamedPipe {
public:
	NamedPipe(std::string d, std::string c) 			
				: daemon(d), client(c)
	{}

	Command ReadCommand() {
		CheckDaemon();
		return static_cast<Command>(fdaemon.get());
	}

	std::string GetLine() {
		CheckDaemon();
		std::string s;
		getline(fdaemon, s);
		return s;
	}

	template <typename T>
	std::ostream& operator<<(const T& obj) {
		CheckClient();
		fclient << obj;
		return fclient;
	}

	template <typename T>
	std::istream& operator>>(T& obj) {
		CheckDaemon();
		fdaemon >> obj;
		return fdaemon;
	}
private:

	void CheckClient() {
		if(!fclient.is_open()) {
			fclient.open(client);
			if(!fclient.is_open()) {
				throw std::runtime_error("Client pipe could not be opened");
			}
		}
	}

	void CheckDaemon() {
		//XXX: Workaround
		if(fdaemon.peek() == -1) fdaemon.ignore();

		if(!fdaemon.good() || !fdaemon.is_open()) {
			fdaemon.close();
            fdaemon.open(daemon);
			if(!fdaemon.is_open()) {
				throw std::runtime_error("Daemon pipe could not be opened");
			}
		}
	}

	std::string daemon;
	std::string client;
	std::ofstream fclient;
	std::ifstream fdaemon;
};