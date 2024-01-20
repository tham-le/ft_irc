#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::names(void)
{
	int idx = 0;
	if (_input[0][0] == '#')
		idx = 1;
	// if (_user.getStatus() != User::ONLINE && _input.size() == 1) {
	// 	_user.printMessage(ERR_NOTJOINEDANYCHANNEL());
	// 	return ;
	// }
	if (_user.getStatus() != User::ONLINE && !_channels.empty()) { //need to update to take only first parameter and skip others
		if (strcmp(_input[0].c_str(),_channels[&_input[0][idx]]->getName().c_str()) == 0) {
				_user.printMessage(RPL_NAMREPLY(_channels[&_input[0][idx]]->getName(), _channels[&_input[0][idx]]->getUsersName())); //replace with later _user.printMessage(353);
				_user.printMessage(RPL_ENDOFNAMES(_channels[&_input[0][idx]]->getName())); //replace with later _user.printMessage(366);
			}
			else
				_user.printMessage(RPL_ENDOFNAMES(_input[0])); //replace later with _user.printMessage(366);
	}
	else if (_user.getStatus() == User::ONLINE) {
		if (_channels.empty() || strcmp(_input[0].c_str(), _user.getLastChannel()->getName().c_str()) == 0) {
			_user.printMessage(RPL_NAMREPLY(_user.getLastChannel()->getName(), _user.getLastChannel()->getUsersName())); //replace with later _user.printMessage(353);
			_user.printMessage(RPL_ENDOFNAMES(_user.getLastChannel()->getName())); //replace later with _user.printMessage(366);
		}
		else if (strcmp(_input[0].c_str(),_channels[&_input[0][idx]]->getName().c_str()) == 0) {
		_user.printMessage(RPL_NAMREPLY(_channels[&_input[0][idx]]->getName(), _channels[&_input[0][idx]]->getUsersName())); //replace with later _user.printMessage(353);
		_user.printMessage(RPL_ENDOFNAMES(_channels[&_input[0][idx]]->getName())); //replace with later _user.printMessage(366);
		}
		else
			_user.printMessage(RPL_ENDOFNAMES(_input[0])); //replace later with _user.printMessage(366);
	}
}
