#include "../includes/Command.hpp"

void	Command::pong(void)
{
	return ;
}

void	Command::ping(void)
{
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		return ;
	}
	_user.printMessage("PONG " + _input[1] + "\r\n");
}
