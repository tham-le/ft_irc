#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::invite(void)
{
	if (_user.getStatus() != User::ONLINE)
	{
		for (unsigned long i = 0; i != _input.size(); i++)
			std::cout << "input[" << i << "] = " << _input[i] << std::endl;
		if (_input[1].empty())
			_user.printMessage(461, _input[0]);
		else if (_input[2][0] != '#' || (_input[2][0] == '#' && !_ircserv.isChannel(_input[2])))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (!_ircserv.isUser(_input[1]))
			_user.printMessage(401, _input[1]); //ERR_NOSUCHNICK
		else if (!_ircserv.getChannel(_input[2])->isUserInChannel(_user))
			_user.printMessage(442, _input[2]); //ERR_NOTONCHANNEL
		else if (!_ircserv.getChannel(_input[2])->isOperator(_user))
			_user.printMessage(482, _input[2]); //ERR_CHANOPRIVSNEEDED
		else if (_ircserv.getChannel(_input[2])->isUserInChannel(_input[1]))
			_user.printMessage(443, _input[1], _input[2]); //ERR_USERONCHANNEL
		else
		{
			std::string msg;
			msg = ":" + _user.getPrefix() + " INVITE " + _input[1] + " " + _input[2] + "\r\n";
			_user.printMessage(msg);
			_user.printMessage(341, _input[1], _input[2]); //RPL_INVITING
		}
	}
	else
	{
		std::cout << "coucou toi" << std::endl;
	}
}
