#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"


void	Command::changeMode(void)
{
	for (unsigned long i = 0; i != _input.size(); i++)
		std::cout << "input[" << i << "] = " << _input[i] << std::endl;

	if (_input[1].empty())
		_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
	else if (_ircserv.isChannel(_input[1]) && _input[2].empty())
	{
		_user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
		_user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
	}
	else if (_ircserv.isChannel(_input[1]) && !_ircserv.getChannel(_input[1])->isOperator(_user))
		_user.printMessage(482, _input[1]); //ERR_CHANOPRIVSNEEDED
	// else if ()
	// else if (!_ircserv.isUser(_input[1]))
	// 	_user.printMessage(401, _input[1]); // ERRNOSUCHNICK
	// else if (_ircserv.getUser(_input[1])->getNickname() != _user.getNickname())
	// 	_user.printMessage(502); //ERR_USERSDONTMATCH

	// if (_input[0].empty() || _input[0].size() < 2) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }
	// int idx = 0;
	// if (_input[0][0] == '#')
	// 	idx = 1;
	// std::map<std::string, Channel *>::iterator it = _channels.find(&_input[0][idx]);
	// if (it == _channels.end()) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }
	// if (_user.getStatus() != User::ONLINE && (_input[0][0] == '-' || _input[0][0] == '+')) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NOTJOINEDANYCHANNEL()); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }

}
