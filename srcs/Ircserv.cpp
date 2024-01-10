#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "Ircserv.hpp"
#include <cstddef>
#include <cstring>
#include <map>
#include <vector>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>   // inet_addr
#include <fcntl.h>
#include <ctime>
#include <unistd.h>

#include <sys/types.h>


#include <iostream>


Ircserv::Ircserv()
{
	std::cout << "Welcome to Ircserv" << std::endl;
}
Ircserv::~Ircserv()
{
	std::cout << "Bye bye" << std::endl;
}

Ircserv::Ircserv(int port, std::string password)
{
	_lastPing = std::time(0);
	(void)port;
	(void)password;

/*	_config.setPort(port);
	_config.setPassword(password);
	_config.setPingInterval(1000);
	_config.setPingTimeout(10 000);
	_config.setMaxClients(100);*/
	std::cout << "Welcome to Ircserv" << std::endl;
}



void			Ircserv::init()
{
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Creating socket() failed");
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed");
	//mode non blocking
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	// addr.sin_port = htons(_config.getPort());
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind port failed");
	if (listen(sockfd, 10) < 0)
		throw std::runtime_error("listen() failed");
	std::cout << "IRC server ready...." << std::endl;
	std::cout << "Listening on port " << 6667 << std::endl;
	std::cout << "Waiting for incoming connections..." << std::endl;
	std::cout << "Press Ctrl-C to exit" << std::endl;
}


void			Ircserv::waitForEvent()
{
	if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
		throw std::runtime_error("poll() failed");
}

void			Ircserv::connectClient()
{
	if (_pollfds[0].revents & POLLIN)
	{
		int fd = accept(_fd, (struct sockaddr *)NULL, NULL);
		if (sockfd < 0)
			throw std::runtime_error("accept() failed");
		std::cout << "New client connected by socket " << sockfd << std::endl;
/*		std::cout << "Client IP address: " << inet_ntoa(((struct sockaddr_in *)&sockfd)->sin_addr) << std::endl;
		std::cout << "Client port      : " << ntohs(((struct sockaddr_in *)&sockfd)->sin_port) << std::endl;
		*/
		if (write(sockfd, "Welcome to Ircserv of CTY Team\n", 31) < 0)
			throw std::runtime_error("write() failed");
		char buffer[1024];
		memset(buffer, 0, 1024);
		if (read(sockfd, buffer, 1024) < 0)
			throw std::runtime_error("read() failed");
		std::cout << "Message received from client: " << buffer << std::endl;

		if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
			throw std::runtime_error("fcntl() failed");
		_pollfds.push_back((pollfd){sockfd, POLLIN, 0});

		for (size_t i = 0; i < _pollfds.size(); i++)
			std::cout << _pollfds[i].fd << std::endl;
		//_users.push_back(new User(sockfd));
	}
}


void			Ircserv::run()
{
	while (1)
	{
		try {
			waitForEvent();
			connectClient();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return ;
		}
	}
}


/*	int pingInterval = _config.getPingInterval();
	int pingTimeout = _config.getPingTimeout();
*/
/*	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)

	{
		if ((*it)->getLastPing() + pingInterval < std::time(0))
			(*it)->ping();
		if ((*it)->getLastPing() + pingTimeout < std::time(0))
			(*it)->disconnect();
	}*/

/*


void			Ircserv::acceptUser()
{
	if (_users.size() == _config.getMaxClients())
		if (shutdown(_pollfds[0].fd, SHUT_RD) < 0)
			throw std::runtime_error("shutdown() failed");

	else
	{
		int sockfd = accept(_pollfds[0].fd, (struct sockaddr *)NULL, NULL);
		if (sockfd < 0)
			throw std::runtime_error("accept() failed");
		if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
			throw std::runtime_error("fcntl() failed");
		_pollfds.push_back(pollfd{sockfd, POLLIN, 0});
		_users.push_back(new User(sockfd));
	}
	
}

void			Ircserv::run();



Config			Ircserv::getConfig() const;
void			Ircserv::setConfig(Config const &config);

void			Ircserv::addUser(User *user);
void			Ircserv::removeUser(User *user);
void			Ircserv::removeUser(std::string const nickname);

bool			Ircserv::isChannel(std::string const channel) const;
void			Ircserv::addChannel(std::string const channel);
void			Ircserv::removeChannel(std::string const channel);
Channel			Ircserv::*getChannel(std::string const channel);
std::map<std::string, Channel>	Ircserv::getChannels() const;

*/

#endif
