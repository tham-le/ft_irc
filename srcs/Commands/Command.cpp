#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

bool	Command::validNickname(std::string const nickname) const
{
	for (unsigned int i = 0; i < nickname.length(); i++) {
		if (!(isalnum(nickname[i]) || nickname[i] == '-' || nickname[i] == '[' || nickname[i] == ']' || nickname[i] == '\\' || nickname[i] == '`' || nickname[i] == '^' || nickname[i] == '{' || nickname[i] == '}' || nickname[i] == '_' || nickname[i] == '|'))
			return false;
	}
	return true;
}

void		Command::admin(void)
{
	(void)_input[0];
	_ircserv.writeToClient(_user.getFd(), "Admin: \n"); //+ to_string(_ircserv.getSocketFd())); //example
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
}

void		Command::info(void)
{
	(void)_input[0];
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
	//std::cout << Ircserv::getInfo() << std::endl; //info of server (name, version, date of creation, etc)
}


void		Command::nickname(void)
{
	std::string nickName;
	if (!validNickname(_input[0])) {
		_user.printMessage(432);
		//_ircserv.writeToClient(_user.getFd(), ERR_ERRONEUSNICKNAME(_input[0]));
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == _input[0]) {
			_user.printMessage(ERR_NICKNAMEINUSE(_input[0]));// replace later with _user.printMessage(433);
			return ;
		}
	}
	_user.printMessage(":" + _user.getPrefix() + " NICK " + _input[0] + "\r\n");
	_user.setNickname(_input[0]);
	nickName = _user.getNickname();
}

void	Command::names(void)
{
	int idx = 0;
	if (_input[0][0] == '#')
		idx = 1;
	// if (_user.getStatus() != User::ONLINE && _input.size() == 1) {
	// 	_user.printMessage(ERR_NOTJOINEDANYCHANNEL());
	// 	return ;
	// }
	if (_user.getStatus() != User::ONLINE && !_channels.empty()) { //need to update to take only first parameter and skip others
		if (strcmp(_input[0].c_str(),_channels[&_input[0][idx]]->getName().c_str()) == 0) {
				_user.printMessage(RPL_NAMREPLY(_channels[&_input[0][idx]]->getName(), _channels[&_input[0][idx]]->getUsersName())); //replace with later _user.printMessage(353);
				_user.printMessage(RPL_ENDOFNAMES(_channels[&_input[0][idx]]->getName())); //replace with later _user.printMessage(366);
			}
			else
				_user.printMessage(RPL_ENDOFNAMES(_input[0])); //replace later with _user.printMessage(366);
	}
	else if (_user.getStatus() == User::ONLINE) {
		if (_channels.empty() || strcmp(_input[0].c_str(), _user.getLastChannel()->getName().c_str()) == 0) {
			_user.printMessage(RPL_NAMREPLY(_user.getLastChannel()->getName(), _user.getLastChannel()->getUsersName())); //replace with later _user.printMessage(353);
			_user.printMessage(RPL_ENDOFNAMES(_user.getLastChannel()->getName())); //replace later with _user.printMessage(366);
		}
		else if (strcmp(_input[0].c_str(),_channels[&_input[0][idx]]->getName().c_str()) == 0) {
		_user.printMessage(RPL_NAMREPLY(_channels[&_input[0][idx]]->getName(), _channels[&_input[0][idx]]->getUsersName())); //replace with later _user.printMessage(353);
		_user.printMessage(RPL_ENDOFNAMES(_channels[&_input[0][idx]]->getName())); //replace with later _user.printMessage(366);
		}
		else
			_user.printMessage(RPL_ENDOFNAMES(_input[0])); //replace later with _user.printMessage(366);
	}
}

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

void	Command::list(void)
{
	_user.printMessage(321);
	if (_input.size() > 1 && _input[0] != _input.back() && _input[0][0] != '#') {
		_user.printMessage(323);
		return ;
	}
	if (_channels.empty())
		_user.printMessage(ERR_NOCHANNNELS());
	else {
		int idx = 0;
		if (_input[0][0] == '#')
			idx = 1;
		if (!_input[0].empty() && _channels.find(&_input[0][idx]) != _channels.end()) {
			//_user.printMessage(322);
			_user.printMessage(RPL_LIST(_channels[&_input[0][idx]]->getName(), to_string(_channels[&_input[0][idx]]->_users.size()),
			_channels[&_input[0][idx]]->getTopic()));
		}
		else if (_input[0].empty()) {
			//_user.printMessage(322);
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_user.printMessage(RPL_LIST(it->first, to_string(it->second->_users.size()), it->second->getTopic()));
		}
	}
	_user.printMessage(323);
}

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
