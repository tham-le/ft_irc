#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::quit(void)
{
	(void)_input[0];
	//_user.setStatus(User::DELETED);
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++) {
		if (std::find(it->second->_operators.begin(), it->second->_operators.end(), &_user) != it->second->_operators.end())
			it->second->removeOperator(_user);
	}
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		it->second->removeUser(_user);
	_ircserv.disconnectClient(_user.getFd());
}
