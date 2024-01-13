#include "../includes/Channel.hpp"
#include <vector>

// Channel::Channel()
// {
// }

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
	if (isUserInChannel(user) == true)
		std::cout << "User is already in the channel." << std::endl;
	else if (_users.size() + 1  >= _maxUser)
		std::cout << "Cannot add user because limits of users is reached";
	else if (isBanned(user) == true)
		std::cout << "Cannot add user because " << user.getNickname() << "is banned." << std::endl;
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
		_users.insert(std::make_pair(user.getFd(), &user));
		std::cout << user.getNickname() << "add in the channel " << this->_name << std::endl;
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
		std::cout << "Cannot remove " << nickname;
		std::cout << "because is the last one operator." << std::endl;
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

void	Channel::Ban(User &user)
{
	if (isUserInChannel(user) == false)
	{
		std::cout << "Cannot ban the user because " << user.getNickname() << " is not in the channel." << std::endl;
		return ;
	}
	if (isOperator(user) && _operators.size() == 1)
	{
		std::cout << "Cannot remove " << user.getNickname();
		std::cout << "because is the last one operator." << std::endl;
	}
	if (isBanned(user) == true)
		std::cout << user.getNickname() << "already ban haha." << std::endl;
	else
	{
		_bannedUsers.push_back(&user);
		removeOperator(user);
		removeUser(user);
		std::cout << user.getNickname() << "is ban now." <<std::endl;
	}
}
void	Channel::Unban(User &user)
{
	std::vector<User *>::iterator it = std::find(_bannedUsers.begin(), _bannedUsers.end(), &user);

	if (it == _bannedUsers.end())
		std::cout << "cannot unban because " << user.getNickname() << " is not ban." << std::endl;
	else
	{
		_bannedUsers.erase(it);
		std::cout << user.getNickname() << "is not ban anymore." <<std::endl;
	}
}
void	Channel::Unban(std::string const nickname)
{
	std::vector<User *>::iterator it;

	for (it = _bannedUsers.begin(); it != _bannedUsers.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			break ;
	}
	if (it == _bannedUsers.end())
	{
		std::cout << "Cannot unban because " << nickname << " is not ban." << std::endl;
		return ;
	}
	_bannedUsers.erase(it);
	std::cout << nickname << " is not ban anymore." <<std::endl;
}

bool	Channel::isBanned(User &user)
{
	std::vector<User *>::iterator it = std::find(_bannedUsers.begin(), _bannedUsers.end(), &user);

	if (it == _bannedUsers.end())
		return (false);
	return (true);
}

/*Si user non connecte est ce qu'on peut l'inviter */
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

	if (isUserInChannel(user))
	{
		std::cout << user.getNickname() << " is not in the channel, cannot to be operator." << std::endl;
		return ;
	}
	if (it != _operators.end())
	{
		std::cout << user.getNickname() << " is already Operator." << std::endl;
		return ;
	}
	_operators.push_back(&user);
	std::cout << user.getNickname() << " is Operator now." << std::endl;
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
