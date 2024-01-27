#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <exception>

#include <iostream>
#include "User.hpp"
#include "Ircserv.hpp"
#include "Channel.hpp"
#include "ReplyCommand.hpp"

class User;
class Ircserv;
class Channel;

class Command
{
public:

	Command(std::string const &msg, User &user, Ircserv &ircserv);

	~Command();

	void		pass(void);
	void		cap(void);
	void		who(void);
	void		ping(void);
	void		pong(void);
	void		privmsg(void);
	void		admin(void);
	void		info(void);
	void		join(void);
	void		nickname(void);
	void		names(void);
	void		part(void);
	void		quit(void);
	void 		list(void);
	void		kick(void);
	void		invite(void);
	void		topic(void);
	void		changeMode(void);
	void 		version(void);
	void		user(void);
	void		whois(void);
	void		modeI(std::string &sign);
	void		modeT(std::string &sign);
	void		modeKpositive(std::string &param);
	void		modeKnegative(void);
	bool		modeLpositive(std::string &param);
	void		modeLnegative(void);
	bool		modeOpositive(std::string &param);
	bool		modeOnegative(std::string &param);
	std::string		parseParamL(std::string str, char c);

	std::vector<std::string>	split(std::string str, char separator);
	void		command();
	void		initCmd();
	bool		validNickname(std::string const nickname) const;
	std::string		toFormat(std::string cmd, std::string str);
	std::string		toFormat(std::string cmd, std::string channel, std::string msg);
	void		joinChannel(Channel *channel);
	void		topicChannel(Channel *channel);
	void		topicCheck(Channel *channel);
	void		modeFind(Channel *channel);
	bool		cInStr(char c, std::string str);
	unsigned long		modeParseParam(std::string sign, char c, unsigned long paramIndex);
	void		joinChannel(std::vector<std::string> channelToJoin, std::vector<std::string> key);


private:
	User						&_user;
	Ircserv						&_ircserv;
	std::vector<std::string>	_input;
	typedef void 				(Command::*FuncType)();
	std::map<std::string, FuncType >		_func;
	std::map<std::string, Channel *>		_channels;
};

template<typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T> &v) {
	for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); it++) {
		os << *it << ' ';
	}
	return os;
}

#endif
