#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void		Command::admin(void)
{
	// if (_input.empty()) {
	// 	_user.printMessage(256, _ircserv.getHostName(), _ircserv.getVersion()); //RPL_ADMINME
	// 	_user.printMessage(258, _ircserv.getHostName()); //RPL_ADMINLOC2
	// 	// _user.printMessage(257, _ircserv.getLocation()); //should we need to add location? RPL_ADMINLOC1
	// 	// _user.printMessage(259, _ircserv.getEmail()); //should we need to add email?
	// 	return ;
	// }
	if (_input.size() == 1)
	{
		_user.printMessage(256, _ircserv.getHostName(), _ircserv.getVersion()); //RPL_ADMINME
		_user.printMessage(258, _ircserv.getHostName()); //RPL_ADMINLOC2
		// _user.printMessage(257, _ircserv.getLocation()); //should we need to add location? RPL_ADMINLOC1
		// _user.printMessage(259, _ircserv.getEmail()); //should we need to add email?
		return ;

	}
	if (strcmp(_input[0].c_str(), _ircserv.getHostName().c_str()) != 0)
		_user.printMessage(402, _input[1]); //ERR_NOSUCHSERVER
}
