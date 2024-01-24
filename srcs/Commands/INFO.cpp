#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"




void		Command::info(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}

	_user.printMessage(371, _ircserv.getVersion(), _ircserv.getStartTime());
	_user.printMessage(371, "This server was created sometime in 2024");
	_user.printMessage(371, "By :");
	_user.printMessage(371, "  - cqin");
	_user.printMessage(371, "  - thi-le");
	_user.printMessage(371, "  - yuboktae");
	_user.printMessage(371, "Based on RFC 2810, 2811, 2812, 2813, 7194");
	_user.printMessage(374);
}
