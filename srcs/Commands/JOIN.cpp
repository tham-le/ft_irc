
#include "../includes/Command.hpp"
#include <string>
#include <vector>

void		Command::joinChannel(Channel *channel)
{
	std::string channelName = channel->getName();
	if (_user.getNbChannelsofUser() + 1 > _user.getMaxChannelofUser())
	{
		_user.printMessage(405, channelName); // ERR_TOOMANYCHANNELS
		return;
	}

	if (channel->getUsers().size() + 1 > channel->getMaxUser())
	{
		_user.printMessage(471, channelName); // ERR_CHANNELISFULL
		return;
	}

	channel->addUser(_user);
	_user.setStatus(User::ONLINE);
	std::string listUsersNames;

	std::map<int, User *> listUsers = channel->getUsers();
	for (std::map<int, User *>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if (channel->isOperator(*it->second))
			listUsersNames += "@" + it->second->getNickname() + " ";
		else
			listUsersNames += it->second->getNickname() + " ";
	}
	for (std::map<int, User *>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
	{
		it->second->printMessage(toFormat("JOIN", channelName) + "\r\n");
		it->second->printMessage(353, "= " + channelName, listUsersNames); // RPL_NAMREPLY
		it->second->printMessage(366, channelName);	// RPL_ENDOFNAMES
	}
	if (channel->getTopic() != "")
	{
		_user.printMessage(332, channelName, channel->getTopic()); //RPL_TOPIC
		_user.printMessage(333, channelName, channel->getTopicTime());//RPL_TOPICWHOTIME
	}
	_user.printMessage(329, channel->getName(), channel->getCreationTime()); //RPL_CREATIONTIME
}

void		Command::joinChannel(std::vector<std::string> channelToJoin, std::vector<std::string> key)
{
	for (unsigned long i = 0; i < channelToJoin.size(); i++)
	{
		if (channelToJoin[i].size() == 0)
			continue;
		if (channelToJoin[i][0] != '#' && channelToJoin[i][0] != '&')
		{
			_user.printMessage(476, channelToJoin[i]); // ERR_BADCHANMASK
			continue;
		}

		if (_ircserv.isChannel(channelToJoin[i])) //if channel exist
		{
			Channel *channel = _ircserv.getChannel(channelToJoin[i]);
			if (channel->getMode() == Channel::INVITE_ONLY)  //if channel is invite only
			{
				if (!channel->isInvited(_user))
				{
					_user.printMessage(473, channel->getName()); // ERR_INVITEONLYCHAN
					continue;
				}
			}
			if (!channel->isUserInChannel(_user))
			{
				if (channel->getKey() != "") //if channel has a key
				{
					if (i >= key.size() && !channel->isGoodKey(key[i]))
					{
						_user.printMessage(475, channel->getName()); // ERR_BADCHANNELKEY
						continue;
					}
					else if (channel->getMode() == Channel::INVITE_ONLY)
						channel->Uninvite(_user);
				}
				joinChannel(channel);
			}
		}
		else //if channel doesn't exist, create it
		{
			//if max channels of server reached
			if (_ircserv.getNbChannels() + 1 > _ircserv.getConfig().getMaxChannels())
			{
				_user.printMessage(405, channelToJoin[i]); // ERR_TOOMANYCHANNELS
				continue;
			}
			_ircserv.addChannel(channelToJoin[i]);
			_ircserv.getChannel(channelToJoin[i])->addOperator(_user);
			joinChannel(_ircserv.getChannel(channelToJoin[i]));
			if (_ircserv.getChannel(channelToJoin[i])->getModeCmd() != "+")
				_user.printMessage(324, channelToJoin[i], _ircserv.getChannel(channelToJoin[i])->getModeCmd()); //RPL_CHANNELMODEIS
		}
	}
}

void		Command::join(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	if (_input.size() < 2)
	{
		_user.printMessage(461, _input[0]); // ERR_NEEDMOREPARAMS
		return ;
	}

	//if JOIN 0 => PART ALL CHANNELS
	if (_input[1] == "0")
	{
		std::map<std::string, Channel *> channels = _ircserv.getChannels();
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			if (it->second->isUserInChannel(_user))
			{
				_input.clear();
				_input.push_back("PART");
				_input.push_back(it->first);
				part();
			}
		}
		return ;
	}

	std::vector<std::string> params(_input.begin() + 1, _input.end());
	std::vector<std::string> channelToJoin;
	std::vector<std::string> key;

<<<<<<< HEAD
	channelToJoin = split(params[0], ',');
	if (params.size() >= 2)
		key = split(params[0], ',');
=======
	if (params.size() == 1)
	{
		channelToJoin = split(params[0], ',');
	}
	else if (params.size() >= 2)
	{
		channelToJoin = split(params[0], ',');
		key = split(params[1], ',');
		// while (params[1].find(',') != std::string::npos)
		// {
		// 	key = split(params[1], ',');
		// 	// key.push_back(params[1].substr(0, params[1].find(',')));
		// 	params[1] = params[1].substr(params[1].find(',') + 1);
		// 	if (params[1].size() == 0)
		// 		break;
		// }
	}
>>>>>>> f93133d40738ad3c91affd022c3b64aa0f5c907d
	if (channelToJoin.size() == 0)
	{
		_user.printMessage(461, _input[0]); // ERR_NEEDMOREPARAMS
		return ;
	}
<<<<<<< HEAD
	joinChannel(channelToJoin, key);
=======

	for (unsigned long i = 0; i < channelToJoin.size(); i++)
	{
		if (channelToJoin[i].size() == 0)
			continue;
		if (channelToJoin[i][0] != '#' && channelToJoin[i][0] != '&')
		{
			_user.printMessage(476, channelToJoin[i]); // ERR_BADCHANMASK
			continue;
		}
		//if channel exist
		if (_ircserv.isChannel(channelToJoin[i]))
		{
			Channel *channel = _ircserv.getChannel(channelToJoin[i]);
			if (channel->getMode() == Channel::INVITE_ONLY)  //if channel is invite only
			{
				if (!channel->isInvited(_user))
				{
					_user.printMessage(473, channel->getName()); // ERR_INVITEONLYCHAN
					continue;
				}
			}
			if (!channel->isUserInChannel(_user))
			{
				if (channel->getKey() != "") //if channel has a key
				{
					std::cout << "KEY = " << channel->getKey() << std::endl;
					std::cout << "KEY SIZE = " << key.size() << std::endl;
					if (i >= key.size() || !channel->isGoodKey(key[i]))
					{
						_user.printMessage(475, channel->getName()); // ERR_BADCHANNELKEY
						continue;
					}
				}
				joinChannel(channel);
			}
		}
		else //if channel doesn't exist, create it
		{
			//if max channels of server reached
			if (_ircserv.getNbChannels() + 1 > _ircserv.getConfig().getMaxChannels())
			{
				_user.printMessage(405, channelToJoin[i]); // ERR_TOOMANYCHANNELS
				continue;
			}
			_ircserv.addChannel(channelToJoin[i]);
			_ircserv.getChannel(channelToJoin[i])->addOperator(_user);
			joinChannel(_ircserv.getChannel(channelToJoin[i]));
			if (_ircserv.getChannel(_input[1])->getModeCmd() != "+")
				_user.printMessage(324, channelToJoin[i], _ircserv.getChannel(_input[1])->getModeCmd()); //RPL_CHANNELMODEIS
			//_user.printMessage(329, channelToJoin[i], to_string(_ircserv.getChannel(channelToJoin[i])->getCreationTime())); // RPL_CREATIONTIME
		}
}

>>>>>>> f93133d40738ad3c91affd022c3b64aa0f5c907d
}
