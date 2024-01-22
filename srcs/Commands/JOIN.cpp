
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
	// for (std::map<int, User *>::iterator i = channel->_users.begin(); i != channel->_users.end(); i++) {
	// 		i->second->printMessage(toFormat("JOIN", channelName));
	// }
	_user.addLastChannel(channel);
	_user.setStatus(User::ONLINE);
	_user.printMessage(toFormat("JOIN", channelName));
	

	//353
	// std::map<int, User *> listUsers = channel->getUsers();
	// std::map<int, User *>::iterator it;
	// std::string s;
	// for (it = listUsers.begin(); it != listUsers.end(); it++)
	// {
	// 	s += "[";
	// 	if (channel->isOperator(it->second->getUsername()))
	// 		s += "@";
	// 	s += it->second->getUsername() + "] ";
	// }
	std::map<int, User *> listUsers = channel->getUsers();
	for (std::map<int, User *>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if (channel->isOperator(it->second->getNickname()))
			_user.printMessage(353, "= " + channelName, "@@" + it->second->getNickname()); // RPL_NAMREPLY
		else
			_user.printMessage(353, "= " + channelName, it->second->getNickname()); // RPL_NAMREPLY
	}
	// _user.printMessage(353, "= " + channelName, s); // RPL_NAMREPLY
	_user.printMessage(366, channelName);	// RPL_ENDOFNAMES
	_user.printMessage(332, channelName, channel->getTopic()); //RPL_TOPIC
	_user.printMessage(332, channelName, channel->getTopicTime());//RPL_TOPICWHOTIME

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
			//_user.printMessage(324, channelToJoin[i], "+nt"); // RPL_CHANNELMODEIS
			//_user.printMessage(329, channelToJoin[i], to_string(_ircserv.getChannel(channelToJoin[i])->getCreationTime())); // RPL_CREATIONTIME
		}
}




	// for (unsigned long i = 0; i < str.size(); i++)
	// {
	// 	// str[i] = toChannelName(str[i]);

	// 	if (!_ircserv.isChannel(str[i]))
	// 	{
	// 		_ircserv.addChannel(str[i]);
	// 		joinChannel(_ircserv.getChannel(str[i]));
	// 		_ircserv.getChannel(str[i])->addOperator(_user);
	// 	}
	// 	else
	// 	{
	// 		if (_user.isInLastChannels(_ircserv.getChannel(str[i])))
	// 			break;
	// 		if (_ircserv.getChannel(str[i])->getMode() == Channel::INVITE_ONLY)
	// 		{
	// 			if (!_ircserv.getChannel(str[i])->isInvited(_user))
	// 			{
	// 				_user.printMessage(473, str[i]);
	// 				flag = 1;
	// 			}
	// 			else if (!key.empty())
	// 			{
	// 				if (i >= key.size() || (i < key.size() && !_ircserv.getChannel(str[i])->isGoodKey(key[i])))
	// 				{
	// 					_user.printMessage(str[i]);
	// 					flag = 1;
	// 				}
	// 			}
	// 		}
	// 		if (!_ircserv.getChannel(str[i])->isUserInChannel(_user) && flag == 0)
	// 			joinChannel(_ircserv.getChannel(str[i]));
	// 	}
	// 	if (_ircserv.getChannel(str[i])->isUserInChannel(_user))
	// 	{

	// 			std::map<int, User *> listUsers;
	// 			listUsers = _user.getLastChannel()->getUsers();
	// 			std::map<int, User *>::iterator it;
	// 			std::string s;
	// 			for (it = listUsers.begin(); it != listUsers.end(); it++)
	// 			{
	// 				s += "[";
	// 				if (_ircserv.getChannel(str[i])->isOperator(it->second->getUsername()))
	// 					s += "@";
	// 				s += it->second->getUsername() + "] ";
	// 			}

	// 			_user.printMessage(toFormat(_input[i], "#" + std::string(str[i])));
	// 			_user.printMessage(353, "= #" + std::string(str[i]), s);
	// 			_user.printMessage(366, "#" + std::string(str[i]));
	// 	}
	// 	/*a supprimer*/
	// 	channelUsers(str[i]);
	// 	/**/
	// }
}
