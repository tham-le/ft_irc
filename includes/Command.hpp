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
	Command(std::string name);

	~Command();

	void		run(int fd, Ircserv &ircserv);
	void		parse(std::string message);
	// std::string	getName() const;

	void		admin(std::string const &message);//Y
	void		info(std::string const &message);
	void		join(std::string const &message);//C
	void		nickname(std::string const &message);//Y
	void		names(std::string const &message);
	void		part(std::string const &message);
	void		quit(std::string const &message);
	void 		list(std::string const &message);
	void		kick(std::string const &message);//C
	void		invite(std::string const &message);//C
	void		topic(std::string const &message);//C
	void		changeMode(std::string const &message);
	bool		isInChannel(std::string const &channel);
	void 		version(std::string const &message);

	void		split(std::string str, char separator);

private:
	std::string _name;
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
