#include "../includes/Command.hpp"
#include "../includes/Ircserv.hpp"

bool	Command::validNickname(std::string const nickname) const
{
	for (unsigned int i = 0; i < nickname.length(); i++) {
		if (!(isalnum(nickname[i]) || nickname[i] == '-' || nickname[i] == '[' \
			 || nickname[i] == ']' || nickname[i] == '\\' || nickname[i] == '`' \
			 || nickname[i] == '^' || nickname[i] == '{' || nickname[i] == '}' \
			 || nickname[i] == '_' || nickname[i] == '|'))
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
	if (!validNickname(_input[1]))
	{

		_user.printMessage(432, _input[1]); //ERR_ERRONEUSNICKNAME 
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == _input[1]) {
			_user.printMessage(443, _input[1]);
			return ;
		}
	}
	//_user.printMessage(":" + _user.getPrefix() + " NICK " + _input[1] + "\r\n");
	_user.printMessage(toFormat("NICK", _input[1]));
	_user.setNickname(_input[1]);
}
