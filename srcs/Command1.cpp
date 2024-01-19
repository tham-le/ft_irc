#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"
#include <cstddef>

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _msg(msg), _user(user), _ircserv(ircserv)
{
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
	// _func["TOPIC"] = &Command::topic;
	// _func["MODE"] = &Command::changeMode;
	// _func["VERSION"] = &Command::version; // -> no channel
}

void		Command::parse(std::string message)
{
	//_input.push_back(message);
	//std::cout << "00 input[0] = " << _input[0] << std::endl;
	_input.clear();
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
		{
			_ircserv.writeToClient(_user.getFd(), ERR_UNKNOWNCOMMAND(_input[0]));
		}
		return ;
	}
	else // if (it == _func.end() && (_user.getStatus() == REGISTRED || _user.getStatus() == ONLINE))
	{
		if (_user.getStatus() == User::ONLINE || _user.getStatus() == User::REGISTERED)
		{
			if (_input.size() > 1)
				(this->*(it->second))(_input[1]);
			else
				(this->*(it->second))("");
		}
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

void		Command::join(std::string const &channel)
{
	int idx = 0;
	int	flag = 0;
	std::vector<std::string> str;
	std::vector<std::string> key;
	if (channel.empty())
	{
		_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0]));
		return ;
	}
	str = split(channel, ',');
	if (_input.size() > 2)
		key = split(_input[2], ',');

	for (unsigned long i = 0; i < str.size(); i++)
	{
		if (str[i][0] == '#')
			idx = 1;

		if (!_ircserv.isChannel(&str[i][idx]))
		{
			_ircserv.addChannel(&str[i][idx]);
			if (_ircserv.getChannel(&str[i][idx])->getUsers().size() + 1 >= _ircserv.getChannel(&str[i][idx])->getMaxUser())
				_ircserv.writeToClient(_user.getFd(), ERR_CHANNELISFULL(std::string(&str[i][idx])));
			_ircserv.getChannel(&str[i][idx])->addUser(_user);
			if (_ircserv.getChannel(&str[i][idx])->isEmptyOperator())
				_ircserv.getChannel(&str[i][idx])->addOperator(_user);
		}
		else
		{
			if (_user.isInLastChannels(_ircserv.getChannel(&str[i][idx])))
				break;
			if (_ircserv.getChannel(&str[i][idx])->getMode() == Channel::INVITE_ONLY)
			{
				if (!_ircserv.getChannel(&str[i][idx])->isInvited(_user))
				{
					_ircserv.writeToClient(_user.getFd(), ERR_INVITEONLYCHAN(std::string(&str[i][idx])));
					flag = 1;
				}
				else if (!key.empty())
				{
					if (i >= key.size() || (i < key.size() && !_ircserv.getChannel(&str[i][idx])->isGoodKey(key[i])))
					{
						_ircserv.writeToClient(_user.getFd(),  ERR_BADCHANNELKEY(std::string(&str[i][idx])) + "\n");
						flag = 1;
					}
				}
			}
			if (!_ircserv.getChannel(&str[i][idx])->isUserInChannel(_user) && flag == 0)
			{
				if (_ircserv.getChannel(&str[i][idx])->getUsers().size() + 1 >= _ircserv.getChannel(&str[i][idx])->getMaxUser())
					_ircserv.writeToClient(_user.getFd(), ERR_CHANNELISFULL(std::string(&str[i][idx])));
				else
					_ircserv.getChannel(&str[i][idx])->addUser(_user);
			}
		}
		if (_ircserv.getChannel(&str[i][idx])->isUserInChannel(_user))
		{
			_user.addChannel(_ircserv.getChannel(&str[i][idx]));
			_user.addLastChannel((_ircserv.getChannel(&str[i][idx])));
			_user.setStatus(User::ONLINE);
			if (_ircserv.getChannel(&str[i][idx])->getTopic() != "")
			{
				_ircserv.writeToClient(_user.getFd(), RPL_TOPIC(std::string(&str[i][idx]), _ircserv.getChannel(&str[i][idx])->getTopic()));
				_ircserv.writeToClient(_user.getFd(), RPL_TOPICWHOTIME(_user.getNickname(), _ircserv.getChannel(&str[i][idx])->getCreationTime()));
			}
		}
	}
}
void		Command::part(std::string const &channel)
{
	std::vector<std::string> listChannel;
	listChannel = split(channel, ',');
	if (_user.getStatus() == User::ONLINE)
	{
		// _user.removeLastChannel();
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
		}
		_user.removeLastChannel();
	}
	else
	{
		if (channel.empty())
			_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0]));
		for (unsigned long i = 0; i < listChannel.size(); i++)
		{
			int j = 0;
			if (listChannel[i][j] == '#')
				j = 1;
			std::cout << &listChannel[i][j] << std::endl;
			if (_ircserv.isChannel(&listChannel[i][j]))
			{
				if (_ircserv.getChannel(&listChannel[i][j])->isUserInChannel(_user))
					_ircserv.writeToClient(_user.getFd(), ERR_NOTONCHANNEL(std::string(&listChannel[i][j])));
			}
			else
			{
				_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(std::string(&listChannel[i][j])));
				return;
			}
		}
	}
}




/*Online sur le channel :
	->non :
			-si pas de argument < 3 : Not enough parameters given
			-si # deuxieme arg : Irssi: Not joined to any channel
			-si argv3 est pas un user du server : a: No such nick/channel
			-si tout ok ,verif si le user lui meme est dans le channel(online) si non : #lol You're not on that channel!
	->oui :
			-verif si le user a kick est dans le channel et si nous somme op remove des list

*/
// void		Command::kick(std::string const &channel)
// {

// }


