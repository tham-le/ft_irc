#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void		Command::info(void)
{
	_user.printMessage(371, _ircserv.getVersion(), _ircserv.getStartTime());
	_user.printMessage(374);
}
