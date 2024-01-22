#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::names(void)
{
	int idx = 0;
	if (_input[1][0] == '#')
		idx = 1;
	// if (_user.getStatus() != User::ONLINE && _input.size() == 1) {
	// 	_user.printMessage(ERR_NOTJOINEDANYCHANNEL());
	// 	return ;
	// }
	if (_user.getStatus() != User::ONLINE && !_channels.empty()) { //need to update to take only first parameter and skip others
		if (strcmp(_input[1].c_str(),_channels[&_input[1][idx]]->getName().c_str()) == 0) {
				_user.printMessage(353, _channels[&_input[1][idx]]->getName(), _channels[&_input[1][idx]]->getUsersName()); //RPL_NAMREPLY
				_user.printMessage(366, _channels[&_input[1][idx]]->getName()); //RPL_ENDOFNAMES
			}
			else
				_user.printMessage(366, _input[1]); //RPL_ENDOFNAMES
	}
	else if (_user.getStatus() == User::ONLINE) {
		if (_channels.empty() || strcmp(_input[1].c_str(), _user.getLastChannel()->getName().c_str()) == 0) {
			_user.printMessage(353, _user.getLastChannel()->getName(), _user.getLastChannel()->getUsersName());
			_user.printMessage(366, _user.getLastChannel()->getName());
		}
		else if (strcmp(_input[1].c_str(),_channels[&_input[1][idx]]->getName().c_str()) == 0) {
		_user.printMessage(353, _channels[&_input[1][idx]]->getName(), _channels[&_input[1][idx]]->getUsersName());
		_user.printMessage(366, _channels[&_input[1][idx]]->getName());
		}
		else
			_user.printMessage(366, _input[1]);
	}
}
