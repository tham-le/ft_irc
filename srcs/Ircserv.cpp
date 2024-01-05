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
	std::cout << "Welcome to Ircserv" << std::endl;
	_config.setPort(port);
	_config.setPassword(password);
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
	//accept
	_fd = accept(sockfd, (struct sockaddr *)NULL, NULL);
	if (_fd < 0)
		throw std::runtime_error("accept() failed");
}

void			Ircserv::run()
{

}

void			Ircserv::run();

void			Ircserv::acceptUser();

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
