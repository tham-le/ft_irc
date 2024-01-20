#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void		Command::part(void)
{
	std::vector<std::string> listChannel;
	listChannel = _input;
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
		if (_input.empty())
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
