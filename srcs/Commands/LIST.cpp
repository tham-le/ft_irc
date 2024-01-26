#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::list(void)
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}
	if (_input.empty())
		return ;
	
	//if LIST has no arguments, return all channels
	if (_input.size() == 1 || (_input.size() == 2 && _input[1].empty())) {
				_user.printMessage(321);
		std::map<std::string, Channel*> channels = _ircserv.getChannels();
		for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
			_user.printMessage(322, it->first, to_string(it->second->getUsers().size()), it->second->getTopic());
		}
		_user.printMessage(323);
		return ;
	}
	//if LIST has arguments, return only those channels



	std::vector<std::string> channels = split(_input[1], ',');
	_user.printMessage(321);
	for (size_t i = 0; i < channels.size(); i++) {
		if (_ircserv.isChannel(channels[i])) {
			_user.printMessage(322, channels[i], to_string(_ircserv.getChannel(channels[i])->getUsers().size()),
			" [" + _ircserv.getChannel(channels[i])->getModeCmd() + "] " + 
			_ircserv.getChannel(channels[i])->getTopic());
		}
		else 
			_user.printMessage(323);
	}
}
