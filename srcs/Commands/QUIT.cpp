#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::quit(void)
{
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++) {
		if (std::find(it->second->_operators.begin(), it->second->_operators.end(), &_user) != it->second->_operators.end())
			it->second->removeOperator(_user);
	}
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		it->second->removeUser(_user);
	_ircserv.disconnectClient(_user.getFd());
	//should send a QUIT message to all clients on the channel: for example dan is exiting form network
	//if there is a reason for quitting, send it to all users on the channel with this reason
}
