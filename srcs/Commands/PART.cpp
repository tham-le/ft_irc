#include "../includes/Command.hpp"
#include "Ircserv.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>

void		Command::part(void)
{
	

	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	if (_user.getStatus() == User::PASSWORD_REQUIRED)
	{
		_user.printMessage(451); //NOT REGISRTED
		return ;
	}
	std::vector<std::string>	listChannel = split(_input[1], ',');


	std::string					reason;
	if (_input.size() > 2)
	{
		std::cout << "reason" << std::endl;
		//concat the rest of input as reason of PART
		for (size_t i = 2; i < _input.size(); i++)
		{
			reason += " ";
			reason += _input[i];
			
		}
	}
	
	std::cout << "reason = " << reason << std::endl;
	std::cout << "listChannel size = " << listChannel.size() << std::endl;
	//std::cout << "listChannel[0] = " << listChannel[0] << std::endl;

	for (unsigned long i = 0; i < listChannel.size(); i++)
	{
		std::cout << "listChannel[" << i << "] = " << listChannel[i] << std::endl;
		if (_ircserv.isChannel(listChannel[i]))
		{
			if (_ircserv.getChannel(listChannel[i])->isUserInChannel(_user))
			{
				_ircserv.getChannel(listChannel[i])->removeUser(_user);
				_user.removeChannel(listChannel[i]);
				//_user.removeLastChannel();
				std::string msg = toFormat("PART",listChannel[i] + reason);
				_user.printMessage(msg);
			}
			else
				_user.printMessage(442, std::string(listChannel[i])); // ERR_NOTONCHANNEL
		}
		else
		{
			_user.printMessage(403, std::string(listChannel[i])); // ERR_NOSUCHCHANNEL
		}
	}
}




	// if (_user.getStatus() == User::ONLINE)
	// {
	// 	_user.setStatus(User::REGISTERED);

	// 	std::map<int, User *> listUsers;
	// 	listUsers = _user.getLastChannel()->getUsers();
	// 	std::map<int, User *>::iterator it;
	// 	for (it = listUsers.begin(); it != listUsers.end(); it++)
	// 	{
	// 		if (it->second->getStatus() == User::ONLINE)
	// 			break;

	// 	}
	// 	if (it == listUsers.end())
	// 	{
	// 		_user.removeChannel(_user.getLastChannel()->getName());
	// 		_ircserv.removeChannel(_user.getLastChannel()->getName());
	// 		_user.getLastChannel()->removeOperator(_user);
	// 	}
	// 	std::string msg;
	// 	msg = toFormat("PART","#" + _user.getLastChannel()->getName());

	// 	std::cout << msg ;
	// 	_user.printMessage(msg);
	// 	_user.removeLastChannel();
	// }
	// else
	// {
	// 	for (unsigned long i = 0; i < listChannel.size(); i++)
	// 	{
	// 		int j = 0;
	// 		if (listChannel[i] == '#')
	// 			j = 1;
	// 		// std::cout << listChannel[i] << std::endl;
	// 		if (_ircserv.isChannel(listChannel[i]))
	// 		{
	// 			if (_ircserv.getChannel(listChannel[i])->isUserInChannel(_user))
	// 				_user.printMessage(442, std::string(listChannel[i]));

	// 		}
	// 		else
	// 		{
	// 			_user.printMessage(403, std::string(listChannel[i]));
	// 			return;
	// 		}
	// 	}
	// }
