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
	if (_input.size() <= 1 || _ircserv.getChannels().empty()) {
		_user.printMessage(323);
		return ;
	}
	std::vector<std::string> channels = split(_input[1], ',');
	for (size_t i = 0; i < channels.size(); i++) {
		if (_ircserv.isChannel(channels[i])) {
			_user.printMessage(322, channels[i], to_string(_ircserv.getChannel(channels[i])->getUsers().size()),
			_ircserv.getChannel(channels[i])->getTopic());
		}
		else 
			_user.printMessage(323);
	}
}