#ifndef USER_HPP
#define USER_HPP


#include <locale>
#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "Command.hpp"


class Channel;
class Command;

class User
{
public:
	User();
	~User();

	User(int fd, struct sockaddr_in addr);

	void	sendMessage(User *user, std::string message);

	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setRealname(std::string realname);
	void	setHostname(std::string hostname);

	std::string	getNickname() const;
	std::string	getUsername() const;
	std::string	getRealname() const;
	std::string	getHostname() const;

	void	setOperator(bool isOperator);
	bool	isOperator() const;

	void	setAway(bool isAway);
	bool	isAway() const;
	void	setAwayMessage(std::string awayMessage);
	std::string	getAwayMessage() const;

private:
	std::map<std::string, void (*)(Command *)> command_function;
	int				_fd;
	std::string		buffer;
	std::vector<std::string>	pendingMessages;
	std::vector<Command *>		CommandQueue;

	std::string		_nickname;
	std::string		_username;
	std::string		_realname;
	std::string		_hostname;

	bool		_isOperator;
	bool		_isAway;
	std::string	_awayMessage;
	

	void	dispatch();
	void	receive(Ircserv *ircserv);
	void	write(std::string message);
	void	pushMessage();
};

#endif
