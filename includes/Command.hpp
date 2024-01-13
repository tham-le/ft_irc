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
	Command();
	Command(std::string msg, int fd);

	~Command();

	void		run(int fd, Ircserv &ircserv);
	void		parse(std::string msg);
	// std::string	getName() const;

	void		admin(std::string const &msg);//Y
	void		info(std::string const &msg);
	void		join(std::string const &msg);//C
	void		nickname(std::string const &msg);//Y
	void		names(std::string const &msg);
	void		part(std::string const &msg);
	void		quit(std::string const &msg);
	void 		list(std::string const &msg);
	void		kick(std::string const &msg);//C
	void		invite(std::string const &msg);//C
	void		topic(std::string const &msg);//C
	void		changeMode(std::string const &msg);
	bool		isInChannel(std::string const &channel);
	void 		version(std::string const &msg);

	void		split(std::string str, char separator);

private:
	std::string _msg;
	int _fd;
	// enum e_type
	// {
	// 	MSG;
	// 	CMD;
	// };
	// e_type _type;
	std::vector <std::string> _lastChannel;
	std::vector<std::string> _input;
	// typedef void (*FuncType)(std::string &);
	typedef void (Command::*FuncType)(std::string const &);
	std::map<std::string, FuncType > _func;
};

#endif
