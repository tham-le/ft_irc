#include "../includes/Command.hpp"
#include "Ircserv.hpp"
#include <cstddef>
#include <string>

void		Command::part(void)
{

	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}

	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	std::vector<std::string>	listChannel = split(_input[1], ',');


	std::string					reason;
	if (_input.size() > 2)
	{
		//concat the rest of input as reason of PART
		for (size_t i = 2; i < _input.size(); i++)
		{
			reason += " ";
			reason += _input[i];

		}
	}

	for (unsigned long i = 0; i < listChannel.size(); i++)
	{
		if (_ircserv.isChannel(listChannel[i]))
		{
			if (_ircserv.getChannel(listChannel[i])->isUserInChannel(_user))
			{
				std::map<int, User *> users = _ircserv.getChannel(listChannel[i])->getUsers();
				std::string msg = toFormat("PART", listChannel[i] + reason);
				for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
					it->second->printMessage(msg + "\r\n");
				_ircserv.getChannel(listChannel[i])->removeUser(_user);
				_user.removeChannel(listChannel[i]);
				if (users.size() == 1)
					_ircserv.removeChannel(listChannel[i]);
			}
			else
				_user.printMessage(442, std::string(listChannel[i])); // ERR_NOTONCHANNEL
		}
		else
			_user.printMessage(403, std::string(listChannel[i])); // ERR_NOSUCHCHANNEL
	}
}
