#include "../includes/User.hpp"
#include <cstddef>
#include <iostream>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>

User::User(int fd, struct sockaddr_in addr, Ircserv *ircserv): _fd(fd)
{
	_ircserv = ircserv;
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		throw std::runtime_error("fcntl() failed");
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	_hostname = inet_ntoa(addr.sin_addr);
	_status = PASSWORD_REQUIRED;
	_isOperator = false;
	_isAway = false;
	_awayMessage = "";
	_buffer = "";
	_nickname = "*";
	_username = "*";
	_realname = "*";
	_ircserv = ircserv;
	_MaxChannelofUser = 50;
	_lastPing = std::time(0);
	_lastPong = std::time(0);

	time_t start = std::time(0);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%d/%m/%y - %H:%M:%S", std::localtime(&start));
	_connectionTime = buffer;
	std::cout << _connectionTime;
	std::cout << ": New user connected by IP: ";
	if (strcmp(_hostname.c_str(), "127.0.0.1") == 0)
		_hostname = "localhost";
	std::cout << _hostname;
	std::cout << " on fd " << fd << std::endl;
	
	printMessage("If you use Irssi, use /CONNECT <host> <port> <password>\r\n");
	printMessage("Irssi does not support /PASS command\r\n");
	// printMessage("If you use Hexchat, use /SERVER <host> <port> <password>\r\n");
	// printMessage("OR Enter password with PASS command and use NICK/USER to register\r\n");
}

User::~User() {}

size_t		User::getMaxChannelofUser() const
{
	return (_MaxChannelofUser);
}

size_t		User::getNbChannelsofUser() const
{
	return (_channels.size());
}

void User::setNickname(std::string nickname) {
	try {
		if (nickname.empty())
			throw std::runtime_error("Nickname cannot be empty");
		for (std::string::iterator it = nickname.begin(); it != nickname.end(); it++)
			if (std::isspace(*it))
				throw std::runtime_error("Nickname cannot contain spaces");
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return ;
	}
	_nickname = nickname;
}


void User::setUsername(std::string username) {
	_username = username;
}

void User::setRealname(std::string realname) {
	_realname = realname;
}

void User::setHostname(std::string hostname) {
	_hostname = hostname;
}

std::string User::getNickname() const {
	return _nickname;
}

std::string User::getUsername() const {
	return _username;
}

std::string User::getRealname() const {
	return _realname;
}

std::string User::getHostname() const {
	return _hostname;
}

int User::getFd() const {
	return _fd;
}

bool User::isOperator() const {
	return _isOperator;
}

void User::setOperator(bool isOperator) {
	_isOperator = isOperator;
}

bool User::isAway() const {
	return _isAway;
}

void User::setAway(bool isAway) {
	_isAway = isAway;
}

void User::setAwayMessage(std::string awayMessage) {
	_awayMessage = awayMessage;
}

std::string User::getAwayMessage() const {
	return _awayMessage;
}

void User::setStatus(e_status status) {
	_status = status;
}

User::e_status User::getStatus() const {
	return _status;
}

void	User::setMode(int mode)
{
	_mode = mode;
}

int		User::getMode() const
{
	return (_mode);
}

void	User::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;
}

void	User::removeChannel(std::string channel)
{
	_channels.erase(channel);
}

void	User::addLastChannel(Channel *channel)
{
	_lastChannels.push_back(channel);
}

void	User::removeLastChannel()
{
	_lastChannels.pop_back();
}

Channel	*User::getLastChannel()
{
	return (_lastChannels[_lastChannels.size() - 1]);
}

bool	User::isInLastChannels(Channel *channel)
{
	std::vector<Channel*>::iterator it = std::find(_lastChannels.begin(), _lastChannels.end(), channel);
	if (it != _lastChannels.end())
		return (true);
	return (false);
}

std::string User::getNameChannels() const
{
	std::string str = "";
	for (std::map<std::string, Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		str += it->first;
		str += " ";
	}
	return (str);
}


std::map<std::string, Channel *>	User::getChannels() const
{
	return (_channels);
}





