#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "Ircserv.hpp"
#include <cstring>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>

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
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		throw std::runtime_error("Creating socket() failed");
	int opt = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed");
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	// addr.sin_port = htons(_config.getPort());
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind port failed: Port already in use");
	if (listen(_sockfd, 10) < 0)
		throw std::runtime_error("listen() failed");
	//std::cout << "Listening on port " << _config.getPort() << std::endl;
	_pollfds.push_back((pollfd){_sockfd, POLLIN, 0});
	//first pollfd is the listening socket
	//the others are the clients
}


void			Ircserv::waitForEvent() {
	int ret = poll(&_pollfds[0], _pollfds.size(), -1);
	if (ret < 0)
		throw std::runtime_error("poll() failed");

}

void			Ircserv::putStrFd(int fd, std::string const &str){
	if (write(fd, str.c_str(), str.size()) < 0)
		throw std::runtime_error("write() failed");
}


void			Ircserv::writeToClient(int fd, std::string const &msg){
	putStrFd(fd, msg);
}
void			Ircserv::writeToAllClients(std::string const &msg)
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		if (it->fd != _sockfd)
			putStrFd(it->fd, msg);
}

void			Ircserv::writeToAllClientsExcept(int fd, std::string const &msg)
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		if (it->fd != _sockfd && it->fd != fd)
			putStrFd(it->fd, msg);
}

static bool containEOL(std::string const &str)
{
	return (str.find("\r\n") != std::string::npos);
}



std::string		Ircserv::readFromClient(int fd)
{
	static std::string buffer[FOPEN_MAX];

	try
	{
		while (!containEOL(buffer))
		{
			char buf[BUFF_SIZE + 1];
			bzero(buf, BUFF_SIZE + 1);
			int bytes = read(fd, buf, BUFF_SIZE);
			if (bytes < 0)
				throw std::runtime_error("read() failed");
			else if (bytes == 0)
				throw std::runtime_error("Client disconnected");
			buffer[fd] += buf;
		}
		std::string msg = buffer[fd].substr(0, buffer[fd].find("\r\n"));
		buffer[fd] = buffer[fd].substr(buffer[fd].find("\r\n") + 2);
		return (msg);
	}
	catch (const std::exception& e)
	{
		std::cerr << "readFromClient() failed: " << e.what() << '\n';
	}
}

void			Ircserv::disconnectClient(int fd)
{
	close(fd);
	_pollfds.erase(_pollfds.begin() + fd);
	_users.erase(_users.begin() + fd);
}

void			Ircserv::disconnectAllClients()
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		if (it->fd != _sockfd)
			disconnectClient(it->fd);
}


void			Ircserv::connectClient()
{
	if (_pollfds[0].revents & POLLIN)
	{
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int fd = accept(_sockfd, (struct sockaddr *)&addr, &len);
		if (fd < 0)
			throw std::runtime_error("accept() failed");
		std::cout << "New client connected by socket " << fd << std::endl;
		_users[fd] = new User(fd, addr);
		
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
			throw std::runtime_error("fcntl() failed");
		writeToClient(sockfd, "Welcome to Ircserv\n");
		_pollfds.push_back((pollfd){fd, POLLIN | POLLOUT, 0});
	}
}


void		Ircserv::handleMessage(int fd, std::string const &msg)
{
	Command *cmd = Command::parse(msg);
	if (cmd == NULL)
	{
		writeToClient(fd, "ERROR :Unknow command\n");
		return ;
	}
	if (cmd->getName() == "QUIT")
	{
		disconnectClient(fd);
		return ;
	}
	cmd->run(fd, *this);

}



void		Ircserv::readFromAllClients()
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->id != _sockfd &&  it->revents & POLLIN)
		{
			std::string msg = readFromClient(it->fd);
			std::cout << "Client " << it->fd << " sent: " << msg << std::endl;
			handleMessage(it->fd, msg);
		}
	}
}




void			Ircserv::run()
{
	while (1)
	{
		try {
			waitForEvent();
			connectClient();
			readFromAllClients();
		}
		catch (const std::exception& e)
		{
			disconnectAllClients();
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
