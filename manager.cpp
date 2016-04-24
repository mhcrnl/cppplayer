#include "manager.h"

#include <sys/stat.h>

Manager::Manager() {
	//Delete pipes, if exist (the program exit abnormaly)
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());

	mkfifo(conf.GetDaemonPipe().c_str(), 0666);
	mkfifo(conf.GetClientPipe().c_str(), 0666);
}

Manager::~Manager() {
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());
}

void Manager::StartServer() {

}