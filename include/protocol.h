#pragma once

#include "commands.h"
#include "config.h"

#include <cstring>
#include <string>
#include <ostream>
#include <istream>

#ifdef _NAMED_PIPE
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#endif

#ifdef _TCP_SOCKET
#include <boost/asio.hpp>
#endif

#ifdef _NAMED_PIPE
//This should only be used for debugging/scripting pourpose
// or if only one client are in use.
class NamedPipe {
public:
	NamedPipe(Config& c) 			
				: conf(c)
	{
		unlink(conf.GetDaemonPipe().c_str());
		unlink(conf.GetClientPipe().c_str());

		if(mkfifo(conf.GetDaemonPipe().c_str(), 0666) == -1)
			throw std::runtime_error(strerror(errno));
		if(mkfifo(conf.GetClientPipe().c_str(), 0666) == -1)
			throw std::runtime_error(strerror(errno));
	}

	~NamedPipe() {
		//Delete pipes
		if(unlink(conf.GetDaemonPipe().c_str()) == -1) 
			throw std::runtime_error(strerror(errno));
		if(unlink(conf.GetClientPipe().c_str()) == -1)
			throw std::runtime_error(strerror(errno));
	}

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

	void Put(char c) {
		fclient.put(c);
	}

	template <typename T>
	std::ostream& operator<<(const T& obj) {
		CheckClient();
		fclient << obj << std::endl;
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
		fclient.close();
		fclient.open(conf.GetClientPipe());
		if(!fclient.is_open()) {
			throw std::runtime_error("Client pipe could not be opened");
		}
	}

	void CheckDaemon() {
		//XXX: Workaround
		if(fdaemon.peek() == -1) fdaemon.ignore();

		if(!fdaemon.good() || !fdaemon.is_open()) {
			fdaemon.close();
            fdaemon.open(conf.GetDaemonPipe());
			if(!fdaemon.is_open()) {
				throw std::runtime_error("Daemon pipe could not be opened");
			}
		}
	}

	Config& conf;
	std::ofstream fclient;
	std::ifstream fdaemon;
};
#endif
//endif _NAMED_PIPE

#ifdef _TCP_SOCKET

//TODO: Accept multiple connections simultaneously
//TODO: Close the connection after send the data 
using boost::asio::ip::tcp;
using namespace boost::asio;

class Tcp {
public:
	Tcp(Config& c)
			: conf(c)
	{
		auto ip = ip::address_v4().from_string(conf.GetBindAddress());
		acceptor = new tcp::acceptor(io_service, tcp::endpoint(ip, conf.GetPortNumber()));
	}

	~Tcp() {
		delete acceptor;
	}

	Command ReadCommand() {
		if(socket != nullptr)
			delete socket;

		socket = new tcp::socket(io_service);
		acceptor->accept(*socket);

		char buffer[1];
        read(*socket, boost::asio::buffer(buffer, 1));
        return static_cast<Command>(buffer[0]);
	}

	std::string GetLine() {
		auto bytes = read_until(*socket, buffer, '\n');
		buffer.commit(bytes);
		std::string line;
		std::getline(is, line);
		return line;
	}

	void Put(char c) {
		os.put(c);
		auto bytes = write(*socket, buffer);
		buffer.consume(bytes);
	}

	template <typename T>
	std::ostream& operator<<(const T& obj) {
		os << obj << std::endl;
		auto bytes = write(*socket, buffer);
		buffer.consume(bytes);
		return os;
	}

	template <typename T>
	std::istream& operator>>(T& obj) {
		auto bytes = boost::asio::read_until(*socket, buffer, '\n');
		buffer.commit(bytes);
		is >> obj;
		return is;
	}
private:
	Config& conf;
	boost::asio::io_service io_service;
	tcp::acceptor* acceptor;
	tcp::socket* socket {nullptr};

	streambuf buffer;
	std::istream is{&buffer};
	std::ostream os{&buffer};
};
#endif
//endif _TCP_SOCKET