#include "../includes/Command.hpp"
#include <ctype.h>
#include <iostream>

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _user(user), _ircserv(ircserv)
{
	initCmd();
	_input = split(msg, ' ');
	//turn input[ 0] to uppercas
	if (_input.size() < 1)
		return ;
	for (size_t i = 0; i < _input[0].size(); i++)
		_input[0][i] = toupper(_input[0][i]);
	command();
}

Command::~Command()
{}

void		Command::initCmd()
{
	_func["PASS"] = &Command::pass;
	_func["CAP"] = &Command::cap;
	_func["PING"] = &Command::ping;
	_func["PONG"] = &Command::pong;

	_func["PRIVMSG"] = &Command::privmsg;
	// _func["NOTICE"] = &Command::notice;
	// _func["WHO"] = &Command::who;

	_func["ADMIN"] = &Command::admin;
	_func["INFO"] = &Command::info; //->no channel
	_func["JOIN"] =  &Command::join;
	_func["NICK"] = &Command::nickname;
	_func["NAMES"] = &Command::names;
	_func["PART"] = &Command::part;
	_func["QUIT"] = &Command::quit;
	_func["LIST"] = &Command::list; //->no channel
	_func["USER"] = &Command::user;
	_func["WHOIS"] = &Command::whois;
	_func["KICK"] = &Command::kick;
	_func["INVITE"] = &Command::invite;
	_func["TOPIC"] = &Command::topic;
	_func["MODE"] = &Command::changeMode;
	_func["VERSION"] = &Command::version; // -> no channel
}


void		Command::command()
{
	std::map<std::string, FuncType >::iterator it = _func.find(_input[0]);
	// if (_input[0] == _user.getMode() == User::PASSWORD_REQUIRED)
	// 	_user.printMessage(451);
	// else
	if (it == _func.end())
	{
		if (_user.getStatus() == User::REGISTERED)
			_user.printMessage(421, _input[0]);
	}
	else if (it != _func.end())
		(this->*it->second)();
}

std::vector<std::string>	Command::split(std::string str, char separator)
{
	std::vector<std::string> input;
	int start = 0;

	for (unsigned int i = 0; i <= str.size(); i++)
	{
		if (str[i] == separator || i == str.size())
		{
			std::string arg;
			arg.append(str, start, i - start);
			input.push_back(arg);
			start = i + 1;
		}
	}
	return (input);
}

std::string		Command::toFormat(std::string cmd, std::string str)
{
	std::string msg = ":" +  _user.getPrefix();
	msg += " " + cmd + " ";
	msg += str + "\r\n";
	return (msg);
}

std::string		Command::toFormat(std::string cmd, std::string channel, std::string msg)
{
	std::string out = ":" +  _user.getPrefix();
	out += " " + cmd + " " + channel + " ";
	out += msg + "\r\n";
	return (out);
}

/*a supp*/
void		Command::channelUsers(std::string channel)
{
	std::map<int, User *> listUsers;
	listUsers = _ircserv.getChannel(channel)->getUsers();
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "List users of channel #" << _ircserv.getChannel(channel)->getName() << std::endl;
	for (std::map<int, User *>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
		std::cout << "*" << it->second->getNickname() << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}
