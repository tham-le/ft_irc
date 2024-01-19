#include "../includes/Channel.hpp"
#include <vector>


Channel::Channel(std::string name): _name(name), _topic("caca"), _mode(PUBLIC), _key(""), _maxUser(100)
{
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

void	Channel::setTopic(std::string const topic)
{
	this->_topic = topic;
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
		if (_mode == INVITE_ONLY)
		{
			if (!isInvited(user))
			{
				// _ircserv.writeToClient(_user.getFd(), _name + ERR_INVITEONLYCHAN + "\n");
				return ;
			}
		}
		_users[user.getFd()] = &user;
		// for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); i++)
		// {
		// 	std::cout << "-List users of channel " << _name << std::endl;
		// 	std::cout << "- " << (i->second)->getHostname() << std::endl;
		// }
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


void			Channel::Invite(User &user)
{
	std::vector<User *>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (isUserInChannel(user) == true)
	{
		std::cout << user.getNickname() << " is already in the channel." << std::endl;
		return ;
	}
	// if (isBanned(user) == true)
	// 	Unban(user);

	if (it != _invitedUsers.end())
		std::cout << user.getNickname() << " is already invited." << std::endl;
	else
	{
		_invitedUsers.push_back(&user);
		std::cout << user.getNickname() << " is now invited." << std::endl;
	}
}

void			Channel::Uninvite(User &user)
{
	std::vector<User *>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (it == _invitedUsers.end())
		std::cout << "Cannot uninvited because "<< user.getNickname() << " is not invited." << std::endl;
	else
		_invitedUsers.erase(it);
}

void			Channel::Uninvite(std::string const nickname)
{
	std::vector<User *>::iterator it;

	for (it = _invitedUsers.begin(); it != _invitedUsers.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			break ;
	}
	if (it == _invitedUsers.end())
		return ;
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
		_operators.push_back(&user);
}


void			Channel::removeOperator(User &user)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), &user);

	_operators.erase(it);
}

void			Channel::removeOperator(std::string const nickname)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), getUser(nickname));

	_operators.erase(it);
	removeUser(nickname);
	// std::cout << nickname << " is not Operator now." << std::endl;
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

void	Channel::setTime()
{
	time_t start = std::time(0);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a %b %d  %H:%M:%S %Y", std::localtime(&start));
	_creationTime = buffer;
}

std::string		Channel::getCreationTime()
{
	return (_creationTime);
}

std::string		Channel::getTopicTime()
{
	return (_setTopicTime);
}
