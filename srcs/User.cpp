#include "../includes/User.hpp"
#include <iostream>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

User::User() {
	_nickname = "";
	_username = "";
	_realname = "";
	_hostname = "";
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
	_nickname = "";
	_username = "";
	_realname = "";
	std::cout << "New user connected by IP: ";
	if (strcmp(_hostname.c_str(), "127.0.0.1") == 0)
		std::cout << "localhost";
	else
		std::cout << _hostname ;
	std::cout << " on fd " << fd << std::endl;
	printMessage("Welcome to the IRC server, as Irrelevant Random Chat  \n");
	printMessage("This server was created " + _ircserv->getStartTime());
	printMessage("\n");


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
