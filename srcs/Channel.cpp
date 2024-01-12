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
	if (isUserInChannel(user) == true)
		std::cout << "User is already in the channel." << std::endl;
	else
	{
		_users.insert({user.getFd(), &user});
		std::cout << user.getNickname() << "add in the channel " << this->_name << std::endl;
	}
}

void	Channel::removeUser(User &user)
{
	if (isUserInChannel(user) == false)
	{
		std::cout << "Cannot remove " << user.getNickname();
		std::cout << "because is not in the channel" << std::endl;
	}
	else
	{
		int fd = user.getFd();
		_users.erase(fd);
		std::cout << user.getNickname() << " is removed from " << this->_name << std::endl;
	}
}

void			Channel::removeUser(std::string const nickname)
{
	int it = _users.find(nickname);

	if (it != _users.end())
	{
		_users.erase(it);
		std::cout << nickname << " is removed from " << this->_name << std::endl;
	}
	else
	{
		std::cout << "Cannot remove " << nickname << " because is not in the channel" << std::endl;
	}
}
User			Channel::getUser(std::string const nickname)
{
	int it = _users.find(nickname);

	if (it != _users.end())
		return (it->second);
	else
		return (NULL);
}
std::map<std::string, User *>	Channel::getUsers() const
{
	return (_users);
}

bool			Channel::isUserInChannel(std::string const nickname) const
{
	return (_users.find(nickname) != _users.end());
}

bool			Channel::isUserInChannel(User const &user) const
{
	int fd = user.getFd();
	if (_users.find(fd) != _users.end())
		return (true);
	return (false);
}


void	Channel::Ban(User &user)
{
	if (isUserInChannel(user) == false)
	{
		std::cout << "Cannot ban the user because " << user.getNickname() << " is not in the channel." << std::endl;
		return ;
	}

	if (user.isBanned() == TRUE)
		std::cout << user.getNickname() << "already ban haha." << std::endl;
	else
	{
		removeUser(user);
		_bannedUsers.push_back(user);
		std::cout << user.getNickname() << "is ban now." <<std::endl;
	}
}
void	Channel::Unban(User &user)
{
	int it = std::find(_bannedUsers.begin(), _bannedUsers.end(), &user);

	if (it == _bannedUsers.end())
		std::cout << "cannot unban because " << user.getNickname() << " is not ban." << std::endl;
	else
	{
		_bannedUsers.erase(it);
		std::cout << user.getNickname() << "is not ban now." <<std::endl;
	}
}
void	Channel::Unban(std::string const nickname)
{
	int it = _bannedUsers.find(nickname);

	if (it != _bannedUsers.end())
	{
		_bannedUsers.erase(it);
		std::cout << user.getNickname() << " is removed from banned users" << std::endl;
	}
	else
	{
		std::cout << "Cannot remove " << nickname << " because not in the banned users." << std::endl;
	}
}

bool	Channel::isBanned(User &user) const
{
	int it = std::find(_bannedUsers.begin(), _bannedUsers.end(), &user);

	if (it == _bannedUsers.end())
		return (false);
	return (true);
}

void			Channel::Invite(User &user)
{
	int it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (isUserInChannel(user) == true)
	{
		std::cout << user.getNickname() << " is already in the channel." << std::endl;
		return ;
	}
	if (isBanned(user) == true)
	{
		std::cout << "Cannot invite user because " << user.getNickname() << " is ban." << std::endl;
		return :
	}

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
	int it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (it == _invitedUsers.end())
		std::cout << "Cannot uninvited because "<< user.getNickname() << " is not invited." << std::endl;
	else
	{
		_invitedUsers.erase(it);
		std::cout << user.getNickname() << " is not in the invited list now." << std::endl;
	}
}
void			Channel::Uninvite(std::string const nickname)
{
	int it = _invitedUsers.find(nickname);

	if (it == _invitedUsers.end())
		std::cout << "Cannot uninvited because "<< user.getNickname() << " is not invited." << std::endl;
	else
	{
		_invitedUsers.erase(it);
		std::cout << user.getNickname() << " is not in the invited list now." << std::endl;
	}
}

bool			Channel::isInvited(User &user) const
{
	int it = std::find(_invitedUsers.begin(), _invitedUsers.end(), &user);

	if (it != _invitedUsers.end())
		return (true);
	return (false);
}
