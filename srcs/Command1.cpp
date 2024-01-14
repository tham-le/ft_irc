#include "../includes/Command.hpp"

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _msg(msg), _user(user), _ircserv(ircserv)
{
	initCmd();
	parse(msg);
	command();
}

Command::~Command()
{}

void		Command::initCmd()
{
	_noFunctionalOnChannel.push_back("ADMIN");
	_noFunctionalOnChannel.push_back("INFO");
	_noFunctionalOnChannel.push_back("LIST");
	_noFunctionalOnChannel.push_back("VERSION");

	_func["ADMIN"] = &Command::admin;
	// _func["INFO"] = &Command::info; //->no channel
	// _func["JOIN"] =  &Command::join;
	// _func["NICK"] = &Command::nickname;
	// _func["NAMES"] = &Command::names;
	// _func["PART"] = &Command::part;
	// _func["QUIT"] = &Command::quit;
	// _func["LIST"] = &Command::list; //->no channel
	// _func["KICK"] = &Command::kick;
	// _func["INVITE"] = &Command::invite;
	// _func["TOPIC"] = &Command::topic;
	// _func["MODE"] = &Command::changeMode;
	// _func["VERSION"] = &Command::version; // -> no channel
}

void		Command::parse(std::string message)
{

	if (message[0] != '/')
	{
		//MSG
	}
	else
	{
		if (message[1] != '/')
			split(&message[1], ' ');
		else
			split(&message[2], ' ');
		// for (unsigned int i = 0; i < _input.size(); i++)
		// {
		// 	std::cout << _input[i] << "." << std::endl;
		// }
	}
}

bool		Command::isCmdNoUse(std::string const str) const
{
	return std::find(_noFunctionalOnChannel.begin(), _noFunctionalOnChannel.end(), str) != _noFunctionalOnChannel.end();
}

void		Command::command()
{
	// std::cout << "hey1" << std::endl;
	std::map<std::string, FuncType>::iterator it = _func.find(_input[0]);
	// std::cout << "hey" << std::endl;

	if (it == _func.end())
	{
		if (_lastChannels.empty())
			std::cout << " Unknown command: " << _input[0] << std::endl;
		return ;
	}
	else
	{
		if ((!_lastChannels.empty() && !isCmdNoUse(_input[0])) || _lastChannels.empty())
			(this->*(it->second))(_input[1]);
	}
}

void		Command::split(std::string str, char separator)
{
	int start = 0;

	for (unsigned int i = 0; i <= str.size(); i++)
	{
		if (str[i] == separator || i == str.size())
		{
			std::string arg;
			arg.append(str, start, i - start);
			_input.push_back(arg);
			start = i + 1;
		}
	}
}

