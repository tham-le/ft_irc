#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::list(void)
{
	_user.printMessage(321);
	if (_input.size() > 1 && _input[0] != _input.back() && _input[0][0] != '#') {
		_user.printMessage(323);
		return ;
	}
	if (_channels.empty())
		_user.printMessage(ERR_NOCHANNNELS());
	else {
		int idx = 0;
		if (_input[0][0] == '#')
			idx = 1;
		if (!_input[0].empty() && _channels.find(&_input[0][idx]) != _channels.end()) {
			//_user.printMessage(322);
			_user.printMessage(RPL_LIST(_channels[&_input[0][idx]]->getName(), to_string(_channels[&_input[0][idx]]->_users.size()),
			_channels[&_input[0][idx]]->getTopic()));
		}
		else if (_input[0].empty()) {
			//_user.printMessage(322);
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_user.printMessage(RPL_LIST(it->first, to_string(it->second->_users.size()), it->second->getTopic()));
		}
	}
	_user.printMessage(323);
}
