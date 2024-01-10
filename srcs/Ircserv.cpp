#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "Ircserv.hpp"
#include <string>
#include <map>
#include <vector>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctime>

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
	//create socket
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	_fd = sockfd;
	//AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
	if (sockfd < 0)
		throw std::runtime_error("Creating socket() failed");
	//setsockopt = set socket options
	//SOL_SOCKET = socket level, SO_REUSEADDR = reuse address
	//&(int){1} = 1, sizeof(int) = size of int
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed");

	//mode non blocking
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");

	
	//bind socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	// addr.sin_port = htons(_config.getPort());
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = INADDR_ANY;

	//forcefully attaching socket to the port
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind port failed");
	//listen
	if (listen(sockfd, 10) < 0)
		throw std::runtime_error("listen() failed");

/*	If there are no incoming connections on the queue, 
	accept() will block until a connection comes in. 
	If you're calling accept() immediately after listen(), 
	and there are no incoming connections, 
	accept() will block indefinitely. 
	To avoid this, you can use select(), 
	poll(), or epoll() to check if there are any pending connections 
	before calling accept().*/
	
	_pollfds.push_back((pollfd){sockfd, POLLIN, 0});
	std::cout << "init done" << std::endl;
}


void			Ircserv::waitForEvent()
{
	int ret = poll(_pollfds.data(), _pollfds.size(), -1);
	if (ret < 0)
		throw std::runtime_error("poll() failed");
}

void			Ircserv::connectClient()
{
	if (_pollfds[0].revents & POLLIN)
	{
		int sockfd = accept(_pollfds[0].fd, (struct sockaddr *)NULL, NULL);
		if (sockfd < 0)
			throw std::runtime_error("accept() failed");
		std::cout << "New client connected by socket " << sockfd << std::endl;
		
		if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
			throw std::runtime_error("fcntl() failed");
		_pollfds.push_back((pollfd){sockfd, POLLIN, 0});
		// _users.push_back(new User(sockfd));
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
