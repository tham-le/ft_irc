#ifndef IRCSERV_HPP
#define IRCSERV_HPP
#include "../includes/Ircserv.hpp"
#include "../includes/Command.hpp"
#include <cstring>
#include <ostream>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <sys/poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include "../includes/Color.h"
#include "User.hpp"
#include <csignal>

bool	stop = false;

class	SigintException : public std::exception
{
	public:
		virtual const char * what() const throw(){
			return ("SIGINT");
		}
};

static void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		stop = true;
	}
}

Ircserv::~Ircserv()
{
	if (_isLog)
	{
		_logFile.close();

	}
	for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		std::cout << "Deleting user " << it->first << std::endl;
		delete it->second;
	}
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		std::cout << "Deleting channel " << it->first << std::endl;
		delete it->second;
	}
	std::cout << "Bye bye" << std::endl;
}

Ircserv::Ircserv(int port, std::string password, std::string opPassword)
{
	_hostName = "Irrelevant Random Chat";
	_version = "1.0";

	_config = Config();
	_config.setPort(port);
	_config.setPassword(password);
	_config.setOpPassword(opPassword);
	_config.setPingInterval(1000);
	_config.setPingTimeout(5000);
	_config.setMaxClients(100);
	time_t start = std::time(0);
	std::cout << "Starting Ircserv..." << std::endl;
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%d/%m/%y - %H:%M:%S", std::localtime(&start));
	std::cout << "Current time: " << buffer << std::endl;
	_startTime = buffer;

	std::string filename = "ircserv.log" + to_string(start);
	
	_logFile.open(filename.c_str());
	
	if (_logFile.is_open())
	{
		std::cout << "Logging to " << filename << std::endl;
		_isLog = true;
	}
	else
	{
		std::cout << "Failed to open log file" << std::endl;
		_isLog = false;
	}
}

void			Ircserv::log(std::string msg)
{
	if (_isLog)
	{
		time_t now = time(0);
		char buffer[100];
		std::strftime(buffer, sizeof(buffer), "%d/%m/%y - %H:%M:%S", std::localtime(&now));
		_logFile << buffer << ": " << msg << std::endl;
		//std::cout << buffer << ": " << msg << std::endl;

	}
}


std::string		Ircserv::getHostName() const
{
	return (_hostName);
}

std::string		Ircserv::getVersion() const
{
	return (_version);
}


User & Ircserv::getUser(int fd) const
{
	std::map<int, User *>::const_iterator it = _users.find(fd);
	if (it == _users.end())
		throw std::runtime_error("User not found");
	return (*it->second);
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
	addr.sin_port = htons(_config.getPort());
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind port failed: Port already in use");
	if (listen(_sockfd, 10) < 0)
		throw std::runtime_error("listen() failed");
	char hostname[1024];
	gethostname(hostname, 1024);
	std::cout << "Listening on port " << _config.getPort() << " on host " << hostname << std::endl;
	_pollfds.push_back((pollfd){_sockfd, POLLIN, 0});
	//first pollfd is the listening socket
	//the others are the clients
}

void			Ircserv::waitForEvent() {
	int ret = poll(&_pollfds[0], _pollfds.size(), -1);
	if (stop == true)
		throw SigintException();
	if (ret < 0)
		throw std::runtime_error("poll() failed");

}

// void			Ircserv::putStrFd(int fd, std::string const &str){
// 	if (write(fd, str.c_str(), str.size()) < 0)
// 		throw std::runtime_error("write() failed");
// }

void			Ircserv::writeToClient(int fd, std::string const &msg){
	User &user = getUser(fd);
	user.printMessage(msg);
}

// void			Ircserv::writeToAllClients(std::string const &msg)
// {
// 	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
// 		if (it->fd != _sockfd)
// 			putStrFd(it->fd, msg);
// }

// void			Ircserv::writeToAllClientsExcept(int fd, std::string const &msg)
// {
// 	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
// 		if (it->fd != _sockfd && it->fd != fd)
// 			putStrFd(it->fd, msg);
// }

/* */
std::string		Ircserv::readFromClient(int fd)
{
	try
	{
		signal(SIGINT, sighandler);
		char buf[4096];
		memset(buf, 0, 4096);

		if (stop == true)
		{
			closeAllSocket();
			return ("");
		}
		if (fd < 0)
			throw std::runtime_error("Invalid fd");

		int bytes = recv(fd, buf, BUFF_SIZE, 0);
		log( "fd " + to_string(fd) +">>>>>>" + buf );

		if (bytes < 0)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				return ("");
			else
				throw std::runtime_error("recv() failed");
		}
		else if (bytes == 0)
			throw DisconnectedUser(fd);
		User	&user = getUser(fd);
		user._buffer += buf;
		std::string delim = "\r\n";
		size_t pos = 0;
		while ((pos = user._buffer.find(delim)) != std::string::npos)
		{
			std::string msg = user._buffer.substr(0, pos);
			user._buffer.erase(0, pos + delim.length());
			if (!msg.length())
				continue ;
			Command cmd(msg, user, *this);
		}

		if (time(0) - user._lastPing > _config.getPingInterval())
		{
			user._lastPing = time(0);
			user.printMessage("PING " + user.getNickname() + "\r\n");
		}
		if (time(0) - user._lastPong > _config.getPingTimeout())
		{
			std::cout << "Client " << fd << " timed out" << std::endl;
			throw DisconnectedUser(fd);
		}
	}
	catch (const DisconnectedUser& e) {
		disconnectClient(e._fd);
	}
	return ("");
}

int				Ircserv::getSocketFd() const
{
	return (_sockfd);
}

void			Ircserv::disconnectClient(int fd)
{	if (fd > 0)
		close(fd);
	std::vector<pollfd>::iterator itfd;
	for (itfd = _pollfds.begin(); itfd != _pollfds.end(); itfd++)
		if (itfd->fd == fd)
			break ;
	if (itfd != _pollfds.end())
		itfd->fd = -42;

	std::map<int, User *>::iterator it = _users.find(fd);
	if (it != _users.end())
	{
		std::cout << "Client " << fd << " disconnected" << std::endl;
		it->second->setStatus(User::DELETED);
		//_users.erase(it);
	}
	else
		std::cout << "Client " << fd << " disconnected" << std::endl;
}

void			Ircserv::closeAllSocket()
{
	std::cout << "Disconnecting all socket" << std::endl;
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd > 0)
		{
			std::cout << "Disconnecting socket " << it->fd << std::endl;
			close(it->fd);

		}
	}

}

std::map<std::string, Channel *>	Ircserv::getChannels() const
{
	return (_channels);
}

std::map<int, User *>	Ircserv::getUsers() const
{
	return (_users);
}

void			Ircserv::connectClient()
{
	if (_pollfds[0].revents & POLLIN)
	{
		if (stop == true)
			throw SigintException();
		if (_users.size() == _config.getMaxClients())
			throw std::runtime_error("Max clients reached");
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int fd = accept(_sockfd, (struct sockaddr *)&addr, &len);
		if (fd < 0)
			throw std::runtime_error("accept() failed");
		std::map<int, User *>::iterator it = _users.find(fd);
		if (it != _users.end())
		{
			if (it->second->getStatus() == User::DELETED)
			{
				delete it->second;
				_users.erase(it);
			}
			else
				throw std::runtime_error("Client already connected");
		}
		_users[fd] = new User(fd, addr, this);
		// User	newUser(fd, addr, this);
		// _users[fd] = &newUser;
		_pollfds.push_back((pollfd){fd, POLLIN | POLLOUT, 0});
	}
}

void		Ircserv::readFromAllClients()
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd > 0 && it->fd != _sockfd &&  it->revents & POLLIN)
			readFromClient(it->fd);

	}
}

void			Ircserv::run()
{
	while (stop == false)
	{
		try {
			signal(SIGINT, sighandler);
			waitForEvent();
			connectClient();
			readFromAllClients();
			if (stop == true)
				throw SigintException();
		}
		catch (const DisconnectedUser& e)
		{
			std::cout << "Client " << e._fd << " disconnected" << std::endl;
			disconnectClient(e._fd);
			std::cerr << e.what() << '\n';
			break ;
		}
		catch (const SigintException& e)
		{

			closeAllSocket();
			std::cerr << e.what() << '\n';
			return ;
		
		}

		catch (const std::exception& e)
		{
			closeAllSocket();
			std::cerr << e.what() << '\n';
			break ;
		}
	}


}

std::string	Ircserv::getStartTime()
{
	return (_startTime);
}

Ircserv::DisconnectedUser::DisconnectedUser(int fd): _fd(fd) {}

Ircserv::DisconnectedUser::~DisconnectedUser()  throw(){}

const char * Ircserv::DisconnectedUser::what() const throw()
{
	return ("User disconnected");
}

void			Ircserv::addChannel(std::string const channel)
{
	_channels[channel] = new Channel(channel);
}

bool			Ircserv::isChannel(std::string const channel) const
{
	if (_channels.find(channel) != _channels.end())
		return (true);
	return (false);
}

void			Ircserv::removeChannel(std::string const channel)
{
	delete _channels[channel];
	_channels.erase(channel);
}

Channel			*Ircserv::getChannel(std::string const channel)
{
	return (_channels[channel]);
}

std::string		Ircserv::getChannelsName() const
{
	std::string channelsName;
	for (std::map<std::string, Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
		channelsName += it->first + " , ";
	return (channelsName);
}

size_t			Ircserv::getNbClients() const
{
	return (_users.size());
}

size_t			Ircserv::getNbChannels() const
{
	return (_channels.size());
}

Config			Ircserv::getConfig() const
{
	return (_config);
}

bool			Ircserv::isUser(std::string nickname) const
{
	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (true);
	}
	return (false);
}

User			*Ircserv::getUser(std::string nickname) const
{
	std::map<int, User *>::const_iterator it;
	for ( it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			break;
	}
	return (it->second);
}

#endif
