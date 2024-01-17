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

void		Command::admin(std::string const &msg)
{
	(void)msg;
	_ircserv.writeToClient(_user.getFd(), "Admin: \n"); //+ to_string(_ircserv.getSocketFd())); //example
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
}

void		Command::info(std::string const &msg)
{
	(void)msg;
	//_ircserv.writeToClient(_user.getFd(), _ircserv.getInfo());
	//std::cout << Ircserv::getInfo() << std::endl; //info of server (name, version, date of creation, etc)
}


void		Command::nickname(std::string const &msg)
{
	std::string nickName;
	if (msg.empty()) {
		// nickName = _user.getNickname(); //it will be the nickename whan i was connected?
		// _ircserv.writeToClient(_user.getFd(), "Your nickname is " + nickName + "\n");
		_ircserv.writeToClient(_user.getFd(), ERR_NONICKNAMEGIVEN());
		return ;
	}
	if (!validNickname(msg)) {
		_ircserv.writeToClient(_user.getFd(), ERR_ERRONEUSNICKNAME(msg));
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == msg) {
			_ircserv.writeToClient(_user.getFd(), ERR_NICKNAMEINUSE(msg));
			return ;
		}
	}
	_user.setNickname(msg);
	nickName = _user.getNickname();
	_ircserv.writeToClient(_user.getFd(), RPL_NICK(nickName));
}

void	Command::names(std::string const &channel)
{
	int idx = 0;
	if (channel[0] == '#')
		idx = 1;
	if (_user.getStatus() != User::ONLINE && _input.size() == 1) {
		_ircserv.writeToClient(_user.getFd(), ERR_NOTJOINEDANYCHANNEL());
		return ;
	}
	else if (_user.getStatus() != User::ONLINE && !_channels.empty() && _input.size() > 1 && channel == _input[1]) { //need to update to take only first parameter and skip others
		if (_channels.find(&channel[idx]) != _channels.end()) {
		_ircserv.writeToClient(_user.getFd(), "Users #" + _channels[&channel[idx]]->getName() + "\n");
		std::vector<User*> operators = _channels[&channel[idx]]->_operators;
		for (unsigned int i = 0; i < operators.size(); i++)
			_ircserv.writeToClient(_user.getFd(), "@" + operators[i]->getNickname() + "\n"); //if there one operator or more??
		}
		else {
			_ircserv.writeToClient(_user.getFd(), channel + ERR_NOSUCHCHANNEL(channel));
			return ;
		}
	}
	else if (_user.getStatus() == User::ONLINE) {
		_ircserv.writeToClient(_user.getFd(), "Users #" + _user.getLastChannel()->getName() + "\n");
		std::vector<User*> operators = _user.getLastChannel()->_operators;
		for (unsigned int i = 0; i < operators.size(); i++)
			_ircserv.writeToClient(_user.getFd(), "@" + operators[i]->getNickname() + "\n"); //if there one operator or more??
		std::map<int, User*> users = _user.getLastChannel()->_users;
		for (unsigned int i = 0; i < users.size(); i++)
			_ircserv.writeToClient(_user.getFd(), users[i]->getNickname() + "\n");
	}
	_ircserv.writeToClient(_user.getFd(), _channels[&channel[idx]]->getName() + "End of /NAMES list\n");
}

void	Command::quit(std::string const &msg)
{
	(void)msg;
	//_user.setStatus(User::DELETED);
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++) {
		if (std::find(it->second->_operators.begin(), it->second->_operators.end(), &_user) != it->second->_operators.end())
			it->second->removeOperator(_user);
	}
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		it->second->removeUser(_user);
	_ircserv.disconnectClient(_user.getFd());
}
void	Command::list(std::string const &channel)
{
	_ircserv.writeToClient(_user.getFd(), RPL_LISTSTART());
	if (_input.size() > 1 && channel != _input.back() && channel[0] != '#') {
		_ircserv.writeToClient(_user.getFd(), RPL_LISTEND());
		return ;
	}
	if (_channels.empty())
		_ircserv.writeToClient(_user.getFd(), ERR_NOCHANNNELS());
	else {
		int idx = 0;
		if (channel[0] == '#')
			idx = 1;
		if (!channel.empty() && _channels.find(&channel[idx]) != _channels.end()) {
			_ircserv.writeToClient(_user.getFd(), RPL_LIST(_channels[&channel[idx]]->getName(), to_string(_channels[&channel[idx]]->_users.size()),
			_channels[&channel[idx]]->getTopic()));
		}
		else if (channel.empty()) {
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_ircserv.writeToClient(_user.getFd(), RPL_LIST(it->first, to_string(it->second->_users.size()), it->second->getTopic()));
		}
	}
	_ircserv.writeToClient(_user.getFd(), RPL_LISTEND());
}

void	Command::changeMode(std::string const &msg)
{
	if (msg.empty() || msg.size() < 2) {
		_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0])); //msg sent to user but not on channel if the user is on a channel
		return ;
	}
	int idx = 0;
	if (msg[0] == '#')
		idx = 1;
	std::map<std::string, Channel *>::iterator it = _channels.find(&msg[idx]);
	if (it == _channels.end()) {
		_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(msg)); //msg sent to user but not on channel if the user is on a channel
		return ;
	}
	if (_user.getStatus() != User::ONLINE && (msg[0] == '-' || msg[0] == '+')) {
		_ircserv.writeToClient(_user.getFd(), ERR_NOTJOINEDANYCHANNEL()); //msg sent to user but not on channel if the user is on a channel
		return ;
	}
}
