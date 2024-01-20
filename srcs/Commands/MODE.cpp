#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::changeMode(void)
{
	if (_input[0].empty() || _input[0].size() < 2) {
		_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
		return ;
	}
	int idx = 0;
	if (_input[0][0] == '#')
		idx = 1;
	std::map<std::string, Channel *>::iterator it = _channels.find(&_input[0][idx]);
	if (it == _channels.end()) {
		_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
		return ;
	}
	if (_user.getStatus() != User::ONLINE && (_input[0][0] == '-' || _input[0][0] == '+')) {
		_ircserv.writeToClient(_user.getFd(), ERR_NOTJOINEDANYCHANNEL()); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
		return ;
	}
}
