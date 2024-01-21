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
	_user.setStatus(User::REGISTERED);
	_user.printMessage(001);
	_user.printMessage(002);
	_user.printMessage(003);
	_user.printMessage(004);
	_user.printMessage(005);

	// _user.printMessage(251, _user.getNickname()); //RPL_LUSERCLIENT
	// _user.printMessage(252, _user.getNickname()); //RPL_LUSEROP
	// _user.printMessage(253, _user.getNickname()); //RPL_LUSERUNKNOWN
	// _user.printMessage(254, _user.getNickname()); //RPL_LUSERCHANNELS
	// _user.printMessage(255, _user.getNickname()); //RPL_LUSERME
	// _user.printMessage(375, _user.getNickname()); //RPL_MOTDSTART
	// _user.printMessage(372, _user.getNickname()); //RPL_MOTD
	// _user.printMessage(376, _user.getNickname()); //RPL_ENDOFMOTD

}
