#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void		Command::info(void)
{
	(void)_input[0];
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
	//std::cout << Ircserv::getInfo() << std::endl; //info of server (name, version, date of creation, etc)
}
