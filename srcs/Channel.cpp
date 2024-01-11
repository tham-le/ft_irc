#include "Channel.hpp"

Channel::Channel()
{
}
Channel::~Channel()
{
}

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

void	Channel::setMode(t_channelMode mode);
{
	this->_mode = mode;
}

t_channelMode	Channel::getMode() const
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
int		Channel::getMaxUser() const
{
	return (_maxUser);
}

void	Channel::addUser(User &user)
{
	if (isUserInChannel(user) == TRUE)
		std::cout << "User is already in the channel." << std::endl;
	else
	{
		_users.insert(pair<int, User *>(user.getFd, user));
		std::cout << user.getNickname() << "add in the channel " << this->_name << std::endl;
	}
}

void	Channel::removeUser(User &user)
{
	if (isUserInChannel(user) == FALSE)
	{
		std::cout << "Cannot remove " << _users[users.getFd].getNickname;
		std::cout << "because is not in the channel" << std::endl;
	}
	else
	{
		int fd = user.getFd();
		_users.erase(_users.find(fd))
		std::cout << user.getNickname() << "is removed from " << this->_name << std::endl;
	}
}

// void			removeUser(std::string const nickname);
// User			*getUser(std::string const nickname);
// std::map<std::string, User *>	getUsers() const;

// bool			isUserInChannel(std::string const nickname) const;
bool			Channel::isUserInChannel(User const &user) const
{
	int fd = user.getFd();
	if (_users.find(fd) != _users.end())
		return (TRUE);
	return (FALSE);
}


void	Channel::Ban(User &user)
{
	if (user.isBanned() == TRUE)
		std::cout << user.getNickname() << "already ban haha." << std::endl;
	else
	{
		_bannedUsers.push_back(user);
		std::cout << user.getNickname() << "is ban now." <<std::endl;
	}

}
void	Channel::Unban(User &user)
{
	if (user.isBanned() == FALSE)
		std::cout << "cannot unban because " << user.getNickname() << " is not ban." << std::endl;
	else
	{
		_bannedUsers.erase(user);
		std::cout << user.getNickname() << "is ban now." <<std::endl;
	}
}
void	Unban(std::string const nickname);
bool	Channel::isBanned(User &user) const
{
	if (this->_bannedUsers[user])
	for (int i = 0; this->_bannedUsers[i]; i++)
	{
		if (this->_bannedUsers[i].getNickname() == user.getNickname())
			return (TRUE);
	}
	return (FALSE);
}

// void			Channel::Invite(User &user);
// void			Channel::Uninvite(User &user);
// void			Channel::Uninvite(std::string const nickname);
bool			Channel::isInvited(User &user) const
{

}
