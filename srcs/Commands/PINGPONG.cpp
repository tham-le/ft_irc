#include "../includes/Command.hpp"

void	Command::pong(void)
{
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	_user._lastPong = std::time(0);
	if (_user._lastPong - _user._lastPing > 60)
		_user.printMessage(toFormat("PING",""));
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
	_user.printMessage(toFormat("PONG",":" + _input[1]));
}
