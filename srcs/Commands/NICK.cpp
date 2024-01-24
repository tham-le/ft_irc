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
			_user.printMessage("Nickname is already in use, changed to " + _input[1]);
		}
	}
	//EVERYTHING IS OK, CHANGE NICKNAME
	_user.printMessage(toFormat("NICK", _input[1]));
	_user.setNickname(_input[1]);
}
