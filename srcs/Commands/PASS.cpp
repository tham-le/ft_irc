#include "../includes/Command.hpp"

void	Command::pass(void)
{
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	if (_user.getStatus() != User::PASSWORD_REQUIRED)
	{
		_user.printMessage(462); //ERR_ALREADYREGISTRED
		return ;
	}
	if (_input[1] != _ircserv.getConfig().getPassword())
	{
		_user.printMessage(464); //ERR_PASSWDMISMATCH
		return ;
	}
	_user.setStatus(User::PASSWORD_MATCH);
	if (_user.getUsername() != "*" && _user.getNickname() != "*")
		Auth();
	else if (_user.getUsername() == "*" && _user.getNickname() != "*")
		_user.printMessage("Username required\r\n");
	else if (_user.getUsername() != "*" && _user.getNickname() == "*")
		_user.printMessage("Nickname required\r\n");
}
