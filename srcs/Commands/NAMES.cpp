#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::names(void)
{

	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	
	int idx = 0;
	if (_input[1][0] == '#')
		idx = 1;
	if (_user.getStatus() != User::ONLINE && _user.getStatus() != User::DELETED && !_channels.empty() && !_ircserv.getChannels().empty()) { //need to update to take only first parameter and skip others
		if (strcmp(_input[1].c_str(),_channels[&_input[1][idx]]->getName().c_str()) == 0) {
				_user.printMessage(353, _channels[&_input[1][idx]]->getName(), _channels[&_input[1][idx]]->getUsersName()); //RPL_NAMREPLY
				_user.printMessage(366, _channels[&_input[1][idx]]->getName()); //RPL_ENDOFNAMES
			}
		else
			_user.printMessage(366, _input[1]); //RPL_ENDOFNAMES
	}
	else if (_user.getStatus() == User::ONLINE) {
		_user.printMessage(353, _channels[&_input[1][idx]]->getName(), _channels[&_input[1][idx]]->getUsersName());
		_user.printMessage(366, _channels[&_input[1][idx]]->getName());
	}
}