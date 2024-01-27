#include "../includes/Command.hpp"

void	Command::names(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	if (_input.size() <= 1 || _ircserv.getChannels().empty()) {
		_user.printMessage(ERR_NOTJOINEDANYCHANNEL()); //RPL_ENDOFNAMES
		return ;
	}
	std::vector<std::string> channels = split(_input[1], ',');
	for (size_t i = 0; i < channels.size(); i++) {
		if (_user.getStatus() != User::ONLINE && _user.getStatus() != User::DELETED && !_ircserv.getChannels().empty()) {
			if (_ircserv.isChannel(channels[i])) {
					_user.printMessage(353, channels[i], _ircserv.getChannel(channels[i])->getUsersName()); //RPL_NAMREPLY
					_user.printMessage(366, channels[i]); //RPL_ENDOFNAMES
				}
			else
				_user.printMessage(366, channels[i]);
		}
		else if (_user.getStatus() == User::ONLINE) {
			_user.printMessage(353, channels[i], _ircserv.getChannel(channels[i])->getUsersName());
			_user.printMessage(366, channels[i]);
		}
	}
}
