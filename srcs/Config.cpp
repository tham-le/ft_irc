#include "Config.hpp"
#include <cstddef>
#include <stdlib.h>
#include <exception>
#include <stdexcept>




Config::Config()
{
	this->_port = 6667;
	this->_password = "password";
	this->_pingInterval = 60;
	this->_pingTimeout = 1000;
	this->_maxClients = 100;
	this->_maxChannels = 100;
}
Config::~Config()
{
}

void	Config::setPort(int port)
{
	this->_port = port;
	if (this->_port < 6660 || this->_port > 7000)
		throw std::invalid_argument("Invalid <port> : <port> must be between 6660 and 7000");
}
void	Config::setPassword(std::string const password)
{
	this->_password = password;
}

int		Config::getPort() const
{
	return (_port);
}

std::string		Config::getPassword() const
{
	return (_password);
}

void	Config::setPingInterval(int const pingInterval)
{
	this->_pingInterval = pingInterval;
}

int		Config::getPingInterval() const
{
	return (_pingInterval);
}

void	Config::setPingTimeout(int const pingTimeout)
{
	this->_pingTimeout = pingTimeout;
}

int		Config::getPingTimeout() const
{
	return (_pingTimeout);
}

void	Config::setMaxClients(int const maxClients)
{
	this->_maxClients = maxClients;
}

size_t		Config::getMaxClients() const
{
	return  (_maxClients);
}

void	Config::setMaxChannels(int const maxChannels)
{
	this->_maxChannels = maxChannels;
}

size_t		Config::getMaxChannels() const
{
	return (_maxChannels);
}

