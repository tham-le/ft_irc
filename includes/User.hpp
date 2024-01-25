#ifndef USER_HPP
#define USER_HPP


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "Command.hpp"
#include "Ircserv.hpp"
#include "Channel.hpp"
#include <ctime>
#include <ReplyCommand.hpp>


class Channel;
class Command;
class Ircserv;

class User
{

public:
	enum	e_status
	{
		PASSWORD_REQUIRED,
		PASSWORD_MATCH,
		REGISTERED,
		ONLINE,
		DELETED
	};
	friend class Command;
	virtual ~User();

	User() {};
	User(int fd, struct sockaddr_in addr, Ircserv *ircserv);

	void	sendMessage(User *user, std::string message);

	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setRealname(std::string realname);
	void	setHostname(std::string hostname);

	std::string	getNickname() const;
	std::string	getUsername() const;
	std::string	getRealname() const;
	std::string	getHostname() const;
	int getFd() const;

	void		setMode(int mode);
	int			getMode() const;


	void	setOperator(bool isOperator);
	bool	isOperator() const;

	void	setAway(bool isAway);
	bool	isAway() const;
	void	setAwayMessage(std::string awayMessage);
	std::string	getAwayMessage() const;

	void	setStatus(e_status status);
	e_status	getStatus() const;


	void	addChannel(Channel *channel);
	void	removeChannel(std::string channel);
	void	addLastChannel(Channel *channel);
	void	removeLastChannel();
	Channel	*getLastChannel();
	bool	isInLastChannels(Channel *channel);

	void	printMessage(std::string str);
	void	printMessage(int code);
	void	printMessage(int code, std::string m1);
	void	printMessage(int code, std::string m1, std::string m2);
	void	printMessage(int code, std::string m1, std::string m2, std::string m3);
	void	printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4);
	void	printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5);
	void	printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6);
	void	printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6, std::string m7);


	std::string	getPrefix() const;
	std::string	getTarget(int code);
	std::string	getNameChannels() const;
	std::map<std::string, Channel *>	getChannels() const;
	size_t		getNbChannelsofUser() const;
	size_t		getMaxChannelofUser() const;

	std::string		_buffer;
	time_t			_lastPing;
	time_t			_lastPong;


private:
	int				_fd;
	Ircserv			*_ircserv;

	//std::string		_pendingMessages;
	std::vector<Command *>		CommandQueue;

	std::string		_nickname;
	std::string		_username;
	std::string		_realname;
	std::string		_hostname;
	e_status		_status;


	bool		_isOperator;
	bool		_isAway;
	std::string	_awayMessage;
	int			_mode;
	int			_MaxChannelofUser;

	std::string		_connectionTime;

	void	receive(Ircserv *ircserv);

	
	std::vector<Channel *> _lastChannels;
	std::map<std::string, Channel *> _channels;
};

#endif
