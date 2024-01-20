#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"
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
	if (_input.size() < 2)
	{
		_user.printMessage(431);
		return ;
	}
	if (!validNickname(_input[1]))
	{
		_user.printMessage(432);
		return ;
	}
	for (std::map<int, User *>::iterator it = _ircserv._users.begin(); it != _ircserv._users.end(); it++) {
		if (it->second->getNickname() == _input[1]) {
			_user.printMessage(443, _input[1]);
			return ;
		}
	}
	_user.printMessage(":" + _user.getPrefix() + " NICK " + _input[1] + "\r\n");
	_user.setNickname(_input[1]);
}
