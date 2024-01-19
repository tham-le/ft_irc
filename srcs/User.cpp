#include "../includes/User.hpp"
#include <iostream>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

User::User() {
	// _nickname = "";
	// _username = "";
	// _realname = "";
	// _hostname = "";
	_isOperator = false;
	_isAway = false;
	_awayMessage = "";
	_buffer = "";
}

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

	time_t start = std::time(0);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%d/%m/%y - %H:%M:%S", std::localtime(&start));
	_connectionTime = buffer;
	std::cout << _connectionTime;
	std::cout << ": New user connected by IP: ";
	if (strcmp(_hostname.c_str(), "127.0.0.1") == 0)
		std::cout << "localhost";
	else
		std::cout << _hostname ;
	std::cout << " on fd " << fd << std::endl;
	printMessage(001);
	printMessage(002);
	printMessage(003);
	printMessage(004);
	printMessage(005);
	// printMessage(":irc.localhost 001 " + _nickname + " :There are 1 users and 0 services on 1 servers");
	// printMessage(":irc.localhost 002 " + _nickname + " :Your host is irc.localhost, running version 1.0");
	// printMessage(":irc.localhost 003 " + _nickname + " :This server was created " + _ircserv->getStartTime());
	// printMessage(":irc.localhost 004 " + _nickname + " irc.localhost 1.0 ao mtov");
	// printMessage(":irc.localhost 005 " + _nickname + " PREFIX=(ov)@+ CHANTYPES=#&+ CHANMODES=,,,");

}

User::~User() {}

void	User::printMessage(std::string str) {
	if (write(_fd, str.c_str(), str.size()) < 0)
		throw std::runtime_error("write() failed");
}

void User::setNickname(std::string nickname) {
	try {
		if (nickname.length() > 9)
			throw std::runtime_error("Nickname too long");
		else if (nickname.length() < 1)
			throw std::runtime_error("Nickname too short");
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

void User::sendMessage(User *user, std::string message) {
	std::cout << "Sending message to " << user->getNickname() << ": " << message << std::endl;
	// send(user->getFd(), message.c_str(), message.length(), 0);
}

void	User::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;

	// for (std::map<std::string, Channel *>::iterator i = _channels.begin(); i != _channels.end(); i++)
	// {
	// 	std::cout << "-List of channel of user " << _nickname << std::endl;
	// 	std::cout << "- " << (i->second)->getName() << std::endl;
	// }
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

std::string	User::getPrefix() const
{
	if (_status == PASSWORD_REQUIRED)
		return ("");
	std::string prefix = _nickname;
	if (_hostname != "")
	{
		if (_username != "")
			prefix += "!" + _username + "@" + _hostname;
		else
			prefix += "!" + _hostname;
	}
	else
		prefix += "!" + _username;
	return (prefix);
}

static std::string	convert3digits(int n)
{
	std::string str = to_string(n);
	while (str.length() < 3)
		str = "0" + str;
	return (str);
}



void	User::printMessage(int code)
{
	std::string str = ":";
	str += getPrefix();
	str += " ";
	str += convert3digits(code);
	str += " ";
	if (_nickname != "")
		str += _nickname;
	else
		str += "*";
	str += " ";

	switch (code) {
	case 001:
		str += RPL_WELCOME(_nickname);
		break;
	case 002:
		str += RPL_YOURHOST(_ircserv->getHostName(), _ircserv->getVersion());
		break;
	case 003:
		str += RPL_CREATED(_ircserv->getStartTime());
		break;
	case 004:
		str += RPL_MYINFO(_ircserv->getHostName(), _ircserv->getVersion(), "ao", "mtov");
		break;
	case 005:
		str += RPL_ISUPPORT();
		break;
	case 311:
		str += RPL_WHOISUSER(_nickname, _username, _hostname, _realname);
		break;
	case 312:
		str += RPL_WHOISSERVER(_nickname, _ircserv->getHostName(), _ircserv->getVersion());
		break;
	case 319:
		str += RPL_WHOISCHANNELS(_nickname, getNameChannels());
		break;
	case 321:
		str += RPL_LISTSTART();
		break;
	case 322:
		str += RPL_LIST(getLastChannel()->getName(), getLastChannel()->getUsersName(), getLastChannel()->getTopic());
		break;
	case 323:
		str += RPL_LISTEND();
		break;
	case 353:
		str += RPL_NAMREPLY(getLastChannel()->getName(), getLastChannel()->getUsersName());
		break;
	case 366:
		str += RPL_ENDOFNAMES(getLastChannel()->getName());
		break;
	case 431:
		str += ERR_NONICKNAMEGIVEN();
		break;
	case 432:
		str += ERR_ERRONEUSNICKNAME();
		break;
	case 461:
		str += ERR_NEEDMOREPARAMS();
		break;
	case 462:
		str += ERR_ALREADYREGISTRED();
		break;
	}
	printMessage(str);
}

