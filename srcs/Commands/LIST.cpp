#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::list(void)
{
	_user.printMessage(321); // RPL_LISTSTART
	if (_input.size() > 1 && _input[1] != _input.back() && _input[1][0] != '#') {
		_user.printMessage(323); // RPL_LISTEND
		return ;
	}
	if (_channels.empty())
		_user.printMessage(ERR_NOCHANNNELS());
	else {
		int idx = 0;
		if (_input[1][0] == '#')
			idx = 1;
		if (!_input[1].empty() && _channels.find(&_input[1][idx]) != _channels.end()) {
			_user.printMessage(322,_channels[&_input[0][idx]]->getName(), to_string(_channels[&_input[1][idx]]->_users.size()),
			_channels[&_input[1][idx]]->getTopic()); // RPL_LIST
		}
		else if (_input[1].empty()) {
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_user.printMessage(322, it->first, to_string(it->second->_users.size()), it->second->getTopic()); // RPL_LIST
		}
	}
	_user.printMessage(323); // RPL_LISTEND
}
