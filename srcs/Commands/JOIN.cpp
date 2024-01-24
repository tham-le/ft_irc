
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

	//_user.addLastChannel(channel);
	_user.setStatus(User::ONLINE);
	//_user.printMessage(toFormat("JOIN", channelName));
	//
	std::string s = toFormat("JOIN", channelName);
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
		it->second->printMessage(s + "\r\n");
		it->second->printMessage(353, "= " + channelName, listUsersNames); // RPL_NAMREPLY
		it->second->printMessage(366, channelName);	// RPL_ENDOFNAMES
	}
	if (channel->getTopic() != "")
	{
		_user.printMessage(332, channelName, channel->getTopic()); //RPL_TOPIC
		_user.printMessage(332, channelName, channel->getTopicTime());//RPL_TOPICWHOTIME
	}
	_user.printMessage(329, channel->getName(), channel->getCreationTime()); //RPL_CREATIONTIME
}

void		Command::join(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED)
	{
		_user.printMessage(451); // ERR_NOTREGISTERED
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

	if (params.size() == 1)
	{
		channelToJoin = split(params[0], ',');
	}
	else if (params.size() >= 2)
	{
		channelToJoin = split(params[0], ',');
		while (params[1].find(',') != std::string::npos)
		{
			key.push_back(params[1].substr(0, params[1].find(',')));
			params[1] = params[1].substr(params[1].find(',') + 1);
			if (params[1].size() == 0)
				break;
		}
	}
	if (channelToJoin.size() == 0)
	{
		_user.printMessage(461, _input[0]); // ERR_NEEDMOREPARAMS
		return ;
	}

	for (unsigned long i = 0; i < channelToJoin.size(); i++)
	{
		//std::cout << "THIS shit channelToJoin[" << i << "] = " << channelToJoin[i] << std::endl;
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
				if (channel->getKey().size() > 0) //if channel has a key
				{
					if (i >= key.size() || (i < key.size() && !channel->isGoodKey(key[i])))
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
			_user.printMessage(toFormat("MODE", channelToJoin[i] + " +nt")); // RPL_CHANNELMODEIS
			_user.printMessage(324, channelToJoin[i], "+o");
			//_user.printMessage(329, channelToJoin[i], to_string(_ircserv.getChannel(channelToJoin[i])->getCreationTime())); // RPL_CREATIONTIME
		}
}

}
