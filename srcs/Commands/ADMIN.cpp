#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void		Command::admin(void)
{
	(void)_input[0];
	_ircserv.writeToClient(_user.getFd(), "Admin: \n"); //+ to_string(_ircserv.getSocketFd())); //example
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
}
