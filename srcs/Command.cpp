#include "../includes/Command.hpp"

Command::Command() : _msg(""), _fd(-1) {}

Command::Command(std::string msg, int fd) : _msg(msg), _fd(fd)
{
	parse(msg);
	_func["ADMIN"] = &Command::admin;
	_func["INFO"] = &Command::info;
	_func["JOIN"] =  &Command::join;
	_func["NICK"] = &Command::nickname;
	_func["NAMES"] = &Command::names;
	_func["PART"] = &Command::part;
	_func["QUIT"] = &Command::quit;
	_func["LIST"] = &Command::list;
	_func["KICK"] = &Command::kick;
	_func["INVITE"] = &Command::invite;
	_func["TOPIC"] = &Command::topic;
	_func["MODE"] = &Command::changeMode;
	_func["VERSION"] = &Command::version;
}

Command::~Command() {}

void		Command::admin(std::string const &message)
{
	(void)message;
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
		for (unsigned int i = 0; i < _input.size(); i++)
		{
			std::cout << _input[i] << "." << std::endl;
		}
	}
}

// void		Command::command()
// {
// 	if (_input[0])
// }

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
/*Si :
1. je suis dans le channel
2. je suis operator ou pas
3. la personne a kick est dans la liste des users du channel */
// void		Channel::kick(User *user, std::string const &message);
// {
// 	if (_isInChannel == TRUE)
// 	{
// 		if (isOperator(user))
// 		{

// 		}
// 		else

// 	}
// }
