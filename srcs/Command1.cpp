#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"
#include <cstddef>

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _msg(msg), _user(user), _ircserv(ircserv)
{
	// std::cout << msg << " lalalalla" << std::endl;
	initCmd();
	parse(msg);
	/*a supp*/
	if (_user.getStatus() == User::PASSWORD_REQUIRED)
		_user.setStatus(User::REGISTERED);
	/**/
	command();
}

Command::~Command()
{}

void		Command::initCmd()
{

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
	// _func["KICK"] = &Command::kick;
	// _func["INVITE"] = &Command::invite;
	_func["TOPIC"] = &Command::topic;
	// _func["MODE"] = &Command::changeMode;
	// _func["VERSION"] = &Command::version; // -> no channel
}

void		Command::parse(std::string message)
{
	// //_input.push_back(message);
	// //std::cout << "00 input[0] = " << _input[0] << std::endl;
	// _input.clear();
	_input = split(message, ' ');
	//std::cout << _input << std::endl;
	// for (unsigned int i = 0; i < _input.size(); i++)
	// 	std::cout << "input[" << i << "] = " << _input[i] << std::endl;
}



void		Command::command()
{
	std::map<std::string, FuncType>::iterator it = _func.find(_input[0]);

	if (it == _func.end())
	{
		if (_user.getStatus() == User::REGISTERED)
			_user.printMessage(421, _input[0]);
	}
	else if (it != _func.end())
	{
		// if (_user.getStatus() == User::PASSWORD_REQUIRED)
		// 	_user.printMessage(451);
		// else
		// {
			if (_input.size() > 1)
				(this->*(it->second))(_input[1]);
			else
				(this->*(it->second))("");
		// }
	}
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
	std::string msg = " :" +  _user.getPrefix();
	msg += " " + cmd + " ";
	msg += str + "\r\n";
	return (msg);
}

std::string		Command::toChannelName(std::string str)
{
	if (str[0] == '#')
		return (&str[1]);
	return (str);
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

/**/


void		Command::joinChannel(Channel *channel)
{
	if (channel->getUsers().size() + 1 > channel->getMaxUser())
	{
		_user.printMessage(471, channel->getName());
		return;
	}
	channel->addUser(_user);
	_user.addChannel(channel);
	_user.addLastChannel(channel);
	_user.setStatus(User::ONLINE);

	// channel->setTopic("caca", channel->getTopicUser());
	// std::string ms = ":" +  _user.getPrefix() + " TOPIC #" + channel->getName() + " :" + channel->getTopic() + "\r\n";
	// _user.printMessage(ms);

	// std::string msg = ":" +  _user.getPrefix() + " TOPIC #" + channel->getName() + "\r\n";
	// _user.printMessage(msg);
	_user.printMessage(332, channel->getName(), channel->getTopic());
	_user.printMessage(333, channel->getTopicUser() ,channel->getTopicTime());

	_user.printMessage(toFormat(_input[0], "#" + channel->getName()));
	_user.printMessage(353, channel->getName(), "lalalla");
	_user.printMessage(366, channel->getName());
}

void		Command::join(std::string const &channel)
{
	if (channel.empty())
	{
		_user.printMessage(461, _input[0]);
		return ;
	}

	std::vector<std::string> str;
	std::vector<std::string> key;
	str = split(channel, ',');
	if (_input.size() > 2)
		key = split(_input[2], ',');

	for (unsigned long i = 0; i < str.size(); i++)
	{
		str[i] = toChannelName(str[i]);

		if (!_ircserv.isChannel(str[i]))
		{
			_ircserv.addChannel(str[i]);
			joinChannel(_ircserv.getChannel(str[i]));
			_ircserv.getChannel(str[i])->addOperator(_user);
		}
		else
		{
			if (_user.isInLastChannels(_ircserv.getChannel(str[i])))
				break;
			if (_ircserv.getChannel(str[i])->getMode() == Channel::INVITE_ONLY)
			{
				if (!_ircserv.getChannel(str[i])->isInvited(_user))
				{
					_user.printMessage(473, str[i]);
					break;
				}
				else
				{
					if (key.empty() || i >= key.size() || (i < key.size() && !_ircserv.getChannel(str[i])->isGoodKey(key[i])))
					{
						_user.printMessage(475, str[i]);
						break;
					}
					else
						joinChannel(_ircserv.getChannel(str[i]));
				}
			}
		}
		/*a supprimer*/
		channelUsers(str[i]);
		/**/
	}
}
void		Command::part(std::string const &channel)
{
	std::vector<std::string> listChannel;
	listChannel = split(channel, ',');
	if (_user.getStatus() == User::ONLINE)
	{
		_user.setStatus(User::REGISTERED);

		std::map<int, User *> listUsers;
		listUsers = _user.getLastChannel()->getUsers();
		std::map<int, User *>::iterator it;
		for (it = listUsers.begin(); it != listUsers.end(); it++)
		{
			if (it->second->getStatus() == User::ONLINE)
				break;

		}
		if (it == listUsers.end())
		{
			_user.removeChannel(_user.getLastChannel()->getName());
			_ircserv.removeChannel(_user.getLastChannel()->getName());
			_user.getLastChannel()->removeOperator(_user);
		}
		std::string msg;
		msg = toFormat("PART","#" + _user.getLastChannel()->getName());

		std::cout << msg ;
		_user.printMessage(msg);
		_user.removeLastChannel();
	}
	else
	{
		if (channel.empty())
			_user.printMessage(461, _input[0]);
		for (unsigned long i = 0; i < listChannel.size(); i++)
		{
			int j = 0;
			if (listChannel[i][j] == '#')
				j = 1;
			// std::cout << &listChannel[i][j] << std::endl;
			if (_ircserv.isChannel(&listChannel[i][j]))
			{
				if (_ircserv.getChannel(&listChannel[i][j])->isUserInChannel(_user))
					_user.printMessage(442, std::string(&listChannel[i][j]));

			}
			else
			{
				_user.printMessage(403, std::string(&listChannel[i][j]));
				return;
			}
		}
	}
}


// void		Command::kick(std::string const &channel)
// {

// }

void		Command::topic(std::string const &str)
{
	if (_user.getStatus() != User::ONLINE)
	{
		if (str.empty())
		{
			_user.printMessage(461, _input[0]);
			return ;
		}

		std::string channel = toChannelName(str);
		if (!_ircserv.isChannel(channel) || str[0] != '#')
		{
			_user.printMessage(403, channel);
			return ;
		}

		if (!_ircserv.getChannel(channel)->isUserInChannel(_user.getNickname()))
		{
			_user.printMessage(442, channel);
			return ;
		}

		if (!_ircserv.getChannel(channel)->isOperator(_user.getNickname()))
		{
			_user.printMessage(482, channel);
			return ;
		}

		if (_input.size() == 2 && _ircserv.getChannel(channel)->getTopic() == "")
		{
			_user.printMessage(331, channel);
			return ;
		}

		if (_input.size() > 2)
		{
			std::string topic;
			topic = _input[2];
			for (unsigned long i = 3; i < _input.size(); i++)
				topic += " " + _input[i];
			std::string msg = "TOPIC #" + channel + " :" + topic + "\r\n";
			_user.printMessage(msg);
			_ircserv.getChannel(channel)->setTopic(topic, _user.getNickname());

		}

	}
}
