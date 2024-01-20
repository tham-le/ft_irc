#include "../includes/Command.hpp"

void		Command::part(void)
{
	std::vector<std::string> listChannel(_input.begin() + 1, _input.end());

	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]);
		return ;
	}
	if (_user.getStatus() == User::REGISTERED)
	{
		_user.printMessage(451);
		return ;
	}
	for (unsigned long i = 0; i < listChannel.size(); i++)
	{
		int j = 0;
		if (listChannel[i][j] == '#')
			j = 1;

		if (_ircserv.isChannel(&listChannel[i][j]))
		{
			if (_ircserv.getChannel(&listChannel[i][j])->isUserInChannel(_user))
			{
				_ircserv.getChannel(&listChannel[i][j])->removeUser(_user);
				_user.removeChannel(&listChannel[i][j]);
				_user.removeLastChannel();
				std::string msg = toFormat("PART","#" + _ircserv.getChannel(&listChannel[i][j])->getName());
				_user.printMessage(msg);
			}
			else
				_user.printMessage(442, std::string(&listChannel[i][j]));
		}
		else
		{
			_user.printMessage(403, std::string(&listChannel[i][j]));
			return;
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
	// 		if (listChannel[i][j] == '#')
	// 			j = 1;
	// 		// std::cout << &listChannel[i][j] << std::endl;
	// 		if (_ircserv.isChannel(&listChannel[i][j]))
	// 		{
	// 			if (_ircserv.getChannel(&listChannel[i][j])->isUserInChannel(_user))
	// 				_user.printMessage(442, std::string(&listChannel[i][j]));

	// 		}
	// 		else
	// 		{
	// 			_user.printMessage(403, std::string(&listChannel[i][j]));
	// 			return;
	// 		}
	// 	}
	// }
