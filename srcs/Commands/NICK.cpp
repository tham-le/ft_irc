#include "../includes/Command.hpp"
#include "../includes/Ircserv.hpp"
#include "User.hpp"

bool	Command::validNickname(std::string const nickname) const
{
	for (unsigned int i = 0; i < nickname.length(); i++) {
		if (!((isalnum(nickname[i]) || nickname[i] == '-' || nickname[i] == '[' \
			 || nickname[i] == ']' || nickname[i] == '\\' || nickname[i] == '`' \
			 || nickname[i] == '^' || nickname[i] == '{' || nickname[i] == '}' \
			 || nickname[i] == '_' || nickname[i] == '|')))
			return false;
	}
	return true;
}

void		Command::nickname(void)
{
	std::string nickName;
	if (_input.size() < 2 || _input[1].empty())
	{
		_user.printMessage(431); //ERR_NONICKNAMEGIVEN 
		return ;
	}
	if (_input[1].length() > 30)
	{
		_user.printMessage("Nickname too long, truncated to 30 characters");
		_input[1] = _input[1].substr(0, 30);
	}
	if (!validNickname(_input[1]))
	{
		_user.printMessage(432, _input[1]); //ERR_ERRONEUSNICKNAME 
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == _input[1] && it->second->getStatus() != User::DELETED) {
			int i = 0;
			while (it->second->getNickname() == _input[1]) {
				_input[1] += "_";
				i++;
				if (i > 10)
				{
					_user.printMessage(433, _input[1]); //ERR_NICKNAMEINUSE
					return ;
				}
			}
			_user.printMessage("Nickname is already in use, trying " + _input[1] + "\r\n");
		}
	}
	//EVERYTHING IS OK, CHANGE NICKNAME
	std::string msg = toFormat("NICK", _input[1]);
	_user.printMessage(msg);
	_user.setNickname(_input[1]);
	std::map<std::string,Channel *> channels = _user.getChannels();
	if (channels.size() > 0)
	{
		for (std::map<std::string,Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			std::map<int, User *> users = it->second->getUsers();
			for (std::map<int, User *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
			{
				if (it2->second->getNickname() != _user.getNickname())
					it2->second->printMessage(msg + "\r\n");
			}
		}
	}

}
