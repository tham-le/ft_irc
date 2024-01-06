#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "Ircserv.hpp"


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
	_config.setPort(port);
	_config.setPassword(password);
	_config.setPingInterval(1000);
	_config.setPingTimeout(10 000);
	_config.setMaxClients(100);
	std::cout << "Welcome to Ircserv" << std::endl;
}

void			Ircserv::init()
{
	//create socket
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
	if (sockfd < 0)
		throw std::runtime_error("socket() failed");
	//setsockopt = set socket options
	//SOL_SOCKET = socket level, SO_REUSEADDR = reuse address
	//&(int){1} = 1, sizeof(int) = size of int
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt() failed");

	//mode non blocking
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");

	
	//bind socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_config.getPort());
	addr.sin_addr.s_addr = INADDR_ANY;

	//forcefully attaching socket to the port
	int binded = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");
	//listen
	int listening = listen(sockfd, 10);
	if (listening < 0)
		throw std::runtime_error("listen() failed");
	if (accept(sockfd, (struct sockaddr *)NULL, NULL) < 0)
		throw std::runtime_error("accept() failed");
	_pollfds.push_back(pollfd{sockfd, POLLIN, 0});

}

void			Ircserv::run()
{
	std::vector<User *> users = getUsers();
	int pingInterval = _config.getPingInterval();
	int pingTimeout = _config.getPingTimeout();

	if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
		throw std::runtime_error("poll() failed");
	if (_pollfds[0].revents & POLLIN)
		acceptUser();
	else
	{
		for (int i = 1; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents & POLLIN)
				users[i]->read();
			else if (_pollfds[i].revents & POLLOUT)
				users[i]->write();
		}
	}

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		if ((*it)->isDisconnected())
		{
			_pollfds.erase(_pollfds.begin() + (it - users.begin()));
			users.erase(it);
		}
	}

	users.clear();
	users = getUsers();
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)

	{
		if ((*it)->getLastPing() + pingInterval < std::time(0))
			(*it)->ping();
		if ((*it)->getLastPing() + pingTimeout < std::time(0))
			(*it)->disconnect();
	}
}


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



#endif
