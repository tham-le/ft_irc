#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>
#include <vector>
#include <functional>


#include "User.hpp"
#include "Ircserv.hpp"
#include "Channel.hpp"

class User;
class Ircserv;
class Channel;

class Command
{
public:

	Command(std::string const &msg, User &user, Ircserv &ircserv);

	~Command();

	void		run(int fd, Ircserv &ircserv);
	void		parse(std::string msg);
	// std::string	getName() const;

	void		admin(std::string const &msg);
	void		info(std::string const &msg);
	void		join(std::string const &channel);
	void		nickname(std::string const &msg);
	void		names(std::string const &msg);
	void		part(std::string const &channel);
	void		quit(std::string const &msg);
	void 		list(std::string const &msg);
	void		kick(std::string const &channel);
	void		invite(std::string const &nickname);
	void		topic(std::string const &msg);
	void		changeMode(std::string const &msg);
	bool		isInChannel(std::string const &channel);
	void 		version(std::string const &msg);

	void		split(std::string str, char separator);
	void		command();
	void		initCmd();

private:
	std::string _msg;
	User &_user;
	Ircserv &_ircserv;
	// enum e_type
	// {
	// 	MSG;
	// 	CMD;
	// };
	// e_type _type;
	std::vector<std::string> _input;
	typedef void (Command::*FuncType)(std::string const &);
	std::map<std::string, FuncType > _func;
	// std::map<std::string, Channel &> _channels;
	// std::vector<Channel *> _channels;
	// std::vector<Channel *> _lastChannels;
	// std::map<std::string, Channel *>	_channels;
	// std::vector<std::string> _noFunctionalOnChannel; //cmd qui ne fonctionne pas lorsque nous sommes sur un channel
};

#endif
