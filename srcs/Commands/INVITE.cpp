#include "../includes/Command.hpp"
#include "User.hpp"

void	Command::invite(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}

	if (_user.getStatus() == User::REGISTERED)
	{
		if (_input.size() < 3 || _input[1].empty())
		{
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
			return;
		}
	}

	if (_input.size() < 3 || _input[1].empty())
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
	else if (_input.size() > 2 && (_input[2][0] != '#' || (_input[2][0] == '#' && !_ircserv.isChannel(_input[2]))))
		_user.printMessage(403, _input[2]); //ERR_NOSUCHCHANNEL
	else if (!_ircserv.isUser(_input[1]) || (_ircserv.isUser(_input[1]) && _ircserv.getUser(_input[1])->getStatus() == User::DELETED))
		_user.printMessage(401, _input[1]); //ERR_NOSUCHNICK
	else if (_input.size() > 2 && !_ircserv.getChannel(_input[2])->isUserInChannel(_user))
		_user.printMessage(442, _input[2]); //ERR_NOTONCHANNEL
	else if (!_ircserv.getChannel(_input[2])->isOperator(_user))
		_user.printMessage(482, _input[2]); //ERR_CHANOPRIVSNEEDED
	else if (_ircserv.getChannel(_input[2])->isUserInChannel(_input[1]))
		_user.printMessage(443, _input[1], _input[2]); //ERR_USERONCHANNEL
	else
	{
		_ircserv.getChannel(_input[2])->Invite(_ircserv.getUser(_input[1]));
		std::string msg;
		msg = ":" + _user.getPrefix() + " INVITE " + _input[1] + " :" + _input[2] + "\r\n";
		_ircserv.getUser(_input[1])->printMessage(msg);
		_user.printMessage(341, _input[1], _input[2]); //RPL_INVITING
	}
}
