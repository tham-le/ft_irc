#include "../includes/Command.hpp"
#include "../includes/ErrorCommand.hpp"

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
		nickName = _user.getNickname(); //it will be the nickename whan i was connected?
		_ircserv.writeToClient(_user.getFd(), "Your nickname is " + nickName + "\n");
		_ircserv.writeToClient(_user.getFd(), ERR_NONICKNAMEGIVEN);
		return ;
	}
	if (!validNickname(msg)) {
		_ircserv.writeToClient(_user.getFd(), msg + ERR_ERRONEUSNICKNAME);
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == msg) {
			_ircserv.writeToClient(_user.getFd(), msg + ERR_NICKNAMEINUSE);
			return ;
		}
	}
	_user.setNickname(msg);
	nickName = _user.getNickname();
	_ircserv.writeToClient(_user.getFd(), "You're now known as " + nickName + "\n");
}

void	Command::names(std::string const &channel)
{
	int idx = 0;
	if (channel[0] == '#')
		idx = 1;
	if (_user.getStatus() != User::ONLINE && _input.size() == 1) {
		_ircserv.writeToClient(_user.getFd(), "Not joined to any channel");
		return ;
	}
	else if (_user.getStatus() != User::ONLINE && !_channels.empty()) {
		if (_channels.find(&channel[idx]) != _channels.end()) {
		_ircserv.writeToClient(_user.getFd(), "Users #" + _channels[&channel[idx]]->getName() + "\n");
		std::vector<User*> operators = _channels[&channel[idx]]->_operators;
		for (unsigned int i = 0; i < operators.size(); i++)
			_ircserv.writeToClient(_user.getFd(), "@" + operators[i]->getNickname() + "\n"); //if there one operator or more??
		}
		else {
			_ircserv.writeToClient(_user.getFd(), channel + ERR_NOSUCHCHANNEL);
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
}

void	Command::quit(std::string const &msg)
{
	(void)msg;
	_user.setStatus(User::DELETED);
	for (std::map<std::string, Channel *>::iterator it = _user._channels.begin(); it != _user._channels.end(); it++)
		it->second->removeUser(_user);
	_ircserv.disconnectClient(_user.getFd());
}

void	Command::list(std::string const &channel)
{
	_ircserv.writeToClient(_user.getFd(), "Channel Topic Users Name\n");
	if (_input.size() > 1 && channel != _input.back() && channel[0] != '#') {
		_ircserv.writeToClient(_user.getFd(), "End of channel list\n");
		return ;
	}
	if (_channels.empty())
		_ircserv.writeToClient(_user.getFd(), ERR_NOCHANNNELS);
	else {
		int idx = 0;
		if (channel[0] == '#')
			idx = 1;
		if (!channel.empty() && _channels.find(&channel[idx]) != _channels.end()) {
			_ircserv.writeToClient(_user.getFd(), _channels[&channel[idx]]->getName() + " " + to_string(_channels[&channel[idx]]->_users.size())
			+ " " + _channels[&channel[idx]]->getTopic() + "\n");
		}
		else if (channel.empty()) {
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_ircserv.writeToClient(_user.getFd(), "- " + it->first + " " + to_string(it->second->_users.size()) + " " + it->second->getTopic() + "\n");
		}
	}
	_ircserv.writeToClient(_user.getFd(), "End of channel list\n");
}

void	changeMode(std::string const &msg)
{
	
}
