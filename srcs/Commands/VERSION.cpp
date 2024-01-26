#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::version(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	_user.printMessage(351, "IRCserv", "1.0.0"); //RPL_VERSION
}
