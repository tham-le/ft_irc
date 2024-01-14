#include "../includes/Channel.hpp"
#include <vector>


Channel::Channel(std::string name): _name(name), _topic(""), _mode(PUBLIC), _key(""), _maxUser(100) {}

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

void	Channel::setMode(t_channelMode mode)
{
	this->_mode = mode;
}

// enum eChannelMode	Channel::getMode() const
// {
// 	return (_mode);
// }

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
int		Channel::getMaxUser() const
{
	return (_maxUser);
}

void	Channel::addUser(User &user)
{
	if (_users.size() + 1  >= _maxUser)
		std::cout << "Cannot add user because limits of users is reached";
	else
	{
		if (_mode == INVITE_ONLY)
		{
			if (!isInvited(user))
			{
				std::cout << "Cannot add user because the channel is on invited only mode and ";
				std::cout << user.getNickname() << " is not invited." << std::endl;
				return ;
			}
		}
		_users[user.getFd()] = &user;
		for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); i++)
		{
			std::cout << "-List users of channel " << _name << std::endl;
			std::cout << "- " << (i->second)->getHostname() << std::endl;
		}
		// std::cout << user.getNickname() << "add in the channel " << this->_name << std::endl;
		if (_mode == INVITE_ONLY)
			Uninvite(user);
	}
}

void	Channel::removeUser(User &user)
{
	if (isUserInChannel(user) == false)
	{
		std::cout << "Cannot remove " << user.getNickname();
		std::cout << "because is not in the channel" << std::endl;
	}
	else if (isOperator(user) && _operators.size() == 1)
	{
		std::cout << "Cannot remove " << user.getNickname();
		std::cout << "because is the last one operator." << std::endl;
	}
	else
	{
		int fd = user.getFd();
		if (isOperator(user))
			removeOperator(user);
		_users.erase(fd);
		std::cout << user.getNickname() << " is removed from " << this->_name << std::endl;
	}
}

void			Channel::removeUser(std::string const nickname)
{
	std::map<int, User *>::iterator it = _users.find(getUser(nickname)->getFd());
	User *user = getUser(nickname);



	if (isUserInChannel(nickname) == false)
	{
		std::cout << "Cannot remove " << nickname;
		std::cout << "because is not in the channel" << std::endl;
	}
	else if (isOperator(*user) && _operators.size() == 1)
	{
		std::cout << "You must be a channel operator" << nickname;
	}
	if (it != _users.end())
	{
		if (isOperator(*user))
			removeOperator(nickname);
		_users.erase(it);
		std::cout << nickname << " is removed from " << this->_name << std::endl;
	}
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

	if (isUserInChannel(user))
	{
		std::cout << "Cannot be a regular user of the channel because ";
		std::cout << user.getNickname() << " is not in the channel." << std::endl;
		return ;
	}
	if (it == _operators.end())
	{
		std::cout << "Cannot remove the user, because " << user.getNickname();
		std::cout << "is not Operator." << std::endl;
		return ;
	}
	_operators.erase(it);
	std::cout << user.getNickname() << " is not Operator anymore." << std::endl;
}

void			Channel::removeOperator(std::string const nickname)
{
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), getUser(nickname));


	if (isUserInChannel(nickname))
	{
		std::cout << "Cannot be a regular user of the channel because ";
		std::cout << nickname << " is not in the channel." << std::endl;
		return ;
	}
	if (it == _operators.end())
	{
		std::cout << "Cannot remove the user, because " << nickname;
		std::cout << "is not Operator." << std::endl;
		return ;
	}
	_operators.erase(it);
	removeUser(nickname);
	std::cout << nickname << " is not Operator now." << std::endl;
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
