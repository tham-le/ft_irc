#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::list(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	_user.printMessage(321);
	if (_input.size() > 1 && _input[1] != _input.back() && _input[1][0] != '#') {
		_user.printMessage(323);
		return ;
	}
	if (_ircserv.getChannels().empty())
		_user.printMessage(ERR_NOCHANNNELS());
	else {
		int idx = 0;
		if (_input[1][0] == '#')
			idx = 1;
		if (!_input[1].empty() && _ircserv.getChannels().find(&_input[1][idx]) != _ircserv.getChannels().end()) {
			_user.printMessage(322, &_input[1][idx], to_string(_ircserv.getChannel(&_input[1][idx])->_users.size()),
			_ircserv.getChannel(&_input[1][idx])->getTopic()); // a refaire
			_user.printMessage(323);
		}
		else if (_input[1].empty()) {
			_user.printMessage(323);
		}
	}
}