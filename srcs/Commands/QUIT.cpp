#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::quit(void)
{
	std::string reason;
	if (_input.size() > 2) {
		for (size_t i = 1; i < _input.size(); i++)
			reason += " " + _input[i];
	}
	if (_user.getStatus() == User::ONLINE)
	{
		for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		{
			if (it->second->isUserInChannel(_user))
				it->second->removeUser(_user);
			if (it->second->isOperator(_user))
				it->second->removeOperator(_user);
			std::string msg = toFormat("QUIT", _ircserv.getChannel(it->first)->getName() + reason);
			std::map<int, User *> users = _ircserv.getChannel(it->first)->getUsers();
			for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
				(*it->second).printMessage(msg + "\r\n");
			if (users.size() == 0)
				_ircserv.removeChannel(it->first);
		}
	}
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++) {
		if (std::find(it->second->_operators.begin(), it->second->_operators.end(), &_user) != it->second->_operators.end())
			it->second->removeOperator(_user);
	}
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		it->second->removeUser(_user);
	_ircserv.disconnectClient(_user.getFd());
}
