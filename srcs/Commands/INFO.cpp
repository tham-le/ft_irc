#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"



void		Command::info(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	_user.printMessage(371, "This server is running an IRC server:" + _ircserv.getVersion(), "Created:" + _ircserv.getStartTime());
	_user.printMessage(371, "By ", "the best team ever:");
	_user.printMessage(371, "  - cqin", "Christine Qin");
	_user.printMessage(371, "  - thi-le", "Tham Le");
	_user.printMessage(371, "  - yuboktae", "Yulia Boktaeva");
	_user.printMessage(371, "Based on RFC 2810, 2811, 2812, 2813, 7194");
	_user.printMessage(374);
}
