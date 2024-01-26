#include "../includes/Command.hpp"

void	Command::pong(void)
{
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	_user._lastPong = std::time(0);
	return ;
}

void	Command::ping(void)
{
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	_user._lastPing = std::time(0);
	_user.printMessage(toFormat("PONG", _ircserv.getHostName() + " :" _input[1]));
}
