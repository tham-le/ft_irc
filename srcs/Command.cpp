#include "../includes/Command.hpp"

void		Command::admin(std::string const &msg)
{
	(void)msg;
	_ircserv.writeToClient(_user.getFd(), "Admin: " + to_string(_ircserv.getSocketFd())); //example
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
	try {
		if (msg.empty()) {
			nickName = _user.getNickname(); //it will be the nickename whan i was connected? 
			_ircserv.writeToClient(_user.getFd(), "Your nickname is " + nickName);
			throw NonNicknameGiven();
		}
		for (unsigned int i = 0; i < msg.length(); i++) {
			if (msg[i] != isalnum(msg[i]) || msg[i] != '-' || msg[i] != '[' || msg[i] != ']' || msg[i] != '\\' || msg[i] != '`' || msg[i] != '^' || msg[i] != '{' || msg[i] != '}' || msg[i] != '_' || msg[i] != '|') {
				throw ErroneusNickname();
				break;
			}
		}
		for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
			if (it->second->getNickname() == msg) {
				throw NicknameInUse();
				break ;
			}
		}
	}
	catch (std::exception &e) {
		_ircserv.writeToClient(_user.getFd(), e.what());
		return ;
	}
	_user.setNickname(msg);
	nickName = _user.getNickname();
	_ircserv.writeToClient(_user.getFd(), "You're now known as " + nickName);
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
	else if (_user.getStatus() != User::ONLINE && !_channels.empty() && _channels.find(&channel[idx]) != _channels.end()) {
		_ircserv.writeToClient(_user.getFd(), "Users #" + _channels[&channel[idx]]->getName());
		std::vector<User*> operators = _channels[&channel[idx]]->_operators;
		for (unsigned int i = 0; i < operators.size(); i++)
			_ircserv.writeToClient(_user.getFd(), "@" + operators[i]->getNickname());
	}
	else if (_user.getStatus() == User::ONLINE && _channels.find(&channel[idx]) != _channels.end()) {
		/*si je suis dans le canal(le dernier canal) et je mets Names ca donne tous les users de chanel
		admin en premier*/
	}
}

void	Command::quit(std::string const &msg)
{
	(void)msg;
	//erase form all channels??
	_ircserv.disconnectClient(_user.getFd());
}

void	Command::list(std::string const &channel)
{
	_ircserv.writeToClient(_user.getFd(), "Channel Users Name");
	if (_input.size() > 1 && channel != _input.back() && channel[0] != '#') {
		_ircserv.writeToClient(_user.getFd(), "End of channel list");
		return ;
	}
	if (_channels.empty())
		std::cout << "No channel created" << std::endl;
	else {
		int idx = 0;
		if (channel[0] == '#')
			idx = 1;
		if (!channel.empty() && _channels.find(&channel[idx]) != _channels.end()) {
			_ircserv.writeToClient(_user.getFd(), _channels[&channel[idx]]->getName() + " " + to_string(_channels[&channel[idx]]->_users.size())
			+ " " + _channels[&channel[idx]]->getTopic());
		}
		else {
			for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
				_ircserv.writeToClient(_user.getFd(), "- " + it->first + " " + to_string(it->second->_users.size()) + " " + it->second->getTopic());
		}
	}
	_ircserv.writeToClient(_user.getFd(), "End of channel list");
}