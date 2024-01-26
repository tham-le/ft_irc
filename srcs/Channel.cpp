#include "../includes/Channel.hpp"
#include <vector>
#include <iostream>


Channel::Channel(std::string name): _name(name), _topic(""), _mode(PUBLIC), _key(""), _modeCmd("+"), _maxUser(100)
{
	_creationTime = setTime();
}

Channel::~Channel(){}

void	Channel::setName(std::string const name)
{
	this->_name = name;
}

std::string		Channel::getName() const
{
	return (_name);
}


void	Channel::setTopic(std::string const topic, std::string const nickname)
{
	this->_topic = topic;
	this->_setTopicTime = setTime();
	this->_setTopicUser = nickname;
}

std::string		Channel::getTopicUser() const
{
	return (_setTopicUser);
}

std::string		Channel::getTopic() const
{
	return (_topic);
}

void	Channel::setMode(e_ChannelMode mode)
{
	this->_mode = mode;
}

Channel::e_ChannelMode	Channel::getMode() const
{
	return (_mode);
}

void	Channel::setKey(std::string const key)
{
	this->_key = key;
}

std::string		Channel::getKey() const
{
	return (_key);
}

void	Channel::setMaxUser(int maxUser)
{
	this->_maxUser = maxUser;
}

unsigned long		Channel::getMaxUser() const
{
	return (_maxUser);
}

void	Channel::addUser(User &user)
{
	_users[user.getFd()] = &user;
	user.addChannel(this);
	if (_mode == INVITE_ONLY)
		Uninvite(user);
}

void	Channel::removeUser(User &user)
{
	int fd = user.getFd();
	if (isOperator(user))
		removeOperator(user);
	_users.erase(fd);
}

void			Channel::removeUser(std::string const nickname)
{
	std::map<int, User *>::iterator it = _users.find(getUser(nickname)->getFd());
	User *user = getUser(nickname);

	if (isOperator(*user))
		removeOperator(nickname);
	_users.erase(it);
}

User*			Channel::getUser(std::string const nickname)
{
	for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (_users.begin()->second);
}

std::map<int, User *>	Channel::getUsers()
{
	return (_users);
}

std::string Channel::getUsersName() const
{
	std::string str = "";
	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		str += it->second->getNickname();
		str += " , ";
	}
	return (str);
}

bool			Channel::isUserInChannel(std::string const nickname) const
{
	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (true);
	}
	return (false);
}

bool			Channel::isUserInChannel(User const &user) const
{
	int fd = user.getFd();
	if (_users.find(fd) != _users.end())
		return (true);
	return (false);
}

void			Channel::Invite(User* user)
{
	_invitedUsers.push_back(user);
}

void			Channel::Uninvite(User &user)
{
	std::vector<User *>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (it == _invitedUsers.end())
		std::cout << "Cannot uninvited because "<< user.getNickname() << " is not invited." << std::endl;
	else
		_invitedUsers.erase(it);
}


bool			Channel::isInvited(User &user)
{
	std::vector<User *>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (it != _invitedUsers.end())
		return (true);
	return (false);
}

bool			Channel::isGoodKey(std::string key)
{
	if (key == _key)
		return (true);
	return (false);
}

void			Channel::addOperator(User &user)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), &user);

	if (it == _operators.end())
	{
		user.setOperator(true);
		_operators.push_back(&user);
	}
}

void			Channel::removeOperator(User &user)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), &user);
	if (it == _operators.end())
		return ;
	_operators.erase(it);
}

void			Channel::removeOperator(std::string const nickname)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), getUser(nickname));
	if (it == _operators.end())
		return ;
	_operators.erase(it);
	// removeUser(nickname);
}

bool			Channel::isOperator(User &user)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), &user);

	if (it == _operators.end())
		return (false);
	return (true);
}

bool			Channel::isOperator(std::string const nickname)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), getUser(nickname));

	if (it == _operators.end())
		return (false);
	return (true);
}

bool			Channel::isEmptyOperator()
{
	if (_operators.empty())
		return (true);
	return (false);
}

std::string		Channel::setTime()
{
	time_t start = std::time(0);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a %b %d  %H:%M:%S %Y", std::localtime(&start));
	return (buffer);
}

std::string		Channel::getCreationTime() const
{
	return (_creationTime);
}

std::string		Channel::getTopicTime() const
{
	return (_setTopicTime);
}

std::string		Channel::getModeCmd() const
{
	return (_modeCmd);
}

void			Channel::setModeCmd(std::string mode)
{
	_modeCmd = mode;
}
