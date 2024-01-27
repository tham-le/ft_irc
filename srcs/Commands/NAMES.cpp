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
	std::vector<std::string> params = split(_input[1], ',');
	for (size_t i = 0; i < params.size(); i++) {
		if (_user.getStatus() != User::ONLINE && _user.getStatus() != User::DELETED && !_ircserv.getChannels().empty()) {
			if (_ircserv.isChannel(params[i])) {
					_user.printMessage(353, params[i], _ircserv.getChannel(params[i])->getUsersName()); //RPL_NAMREPLY
					_user.printMessage(366, params[i]); //RPL_ENDOFNAMES
				}
		}
		else if (_user.getStatus() == User::ONLINE) {
			if (_ircserv.isChannel(params[i])) {
				if (!_ircserv.getChannel(params[i])->isUserInChannel(_user))
					_user.printMessage(366, params[i]);
				else {
					_user.printMessage(353, params[i], _ircserv.getChannel(params[i])->getUsersName());
					_user.printMessage(366, params[i]);
				}
			}
		}
	}
}
