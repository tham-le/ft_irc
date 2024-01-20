#include "../includes/Command.hpp"
#include <vector>

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
}

void		Command::join(void)
{
	if (_input.size() < 2)
	{
		_user.printMessage(461, _input[0]);
		return ;
	}

	int	flag = 0;
	std::vector<std::string> str(_input.begin() + 1, _input.end());
	
	std::vector<std::string> channelToJoin;
	std::vector<std::string> key;
	

	for (unsigned long i = 0; i < str.size(); i++)
	{
		// str[i] = toChannelName(str[i]);

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
					flag = 1;
				}
				else if (!key.empty())
				{
					if (i >= key.size() || (i < key.size() && !_ircserv.getChannel(str[i])->isGoodKey(key[i])))
					{
						_user.printMessage(str[i]);
						flag = 1;
					}
				}
			}
			if (!_ircserv.getChannel(str[i])->isUserInChannel(_user) && flag == 0)
				joinChannel(_ircserv.getChannel(str[i]));
		}
		if (_ircserv.getChannel(str[i])->isUserInChannel(_user))
		{

				std::map<int, User *> listUsers;
				listUsers = _user.getLastChannel()->getUsers();
				std::map<int, User *>::iterator it;
				std::string s;
				for (it = listUsers.begin(); it != listUsers.end(); it++)
				{
					s += "[";
					if (_ircserv.getChannel(str[i])->isOperator(it->second->getUsername()))
						s += "@";
					s += it->second->getUsername() + "] ";
				}

				_user.printMessage(toFormat(_input[i], "#" + std::string(str[i])));
				_user.printMessage(353, "= #" + std::string(str[i]), s);
				_user.printMessage(366, "#" + std::string(str[i]));			
		}
		/*a supprimer*/
		channelUsers(str[i]);
		/**/
	}
}
