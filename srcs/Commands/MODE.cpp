#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

bool	Command::cInStr(char c, std::string str)
{
	size_t found = str.find(c);
	if (found != std::string::npos)
		return (true);
	return (false);
}

std::string		Command::parseParamL(std::string str, char c)
{
	for (unsigned long i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
		{
			if (i == 0)
			{
				std::string flag(1, c);
				_user.printMessage(500, _input[1], flag); // ERR_CMODE
				return (0);
			}
			else
				str = str.substr(0, i);
		}
	}
	return (str);
}

unsigned long	Command::modeParseParam(std::string sign, char c, unsigned long paramIndex)
{
	if (c == 'i')
	{
		modeI(sign);
		return (--paramIndex);
	}
	else if (c == 't')
	{
		modeT(sign);
		return (--paramIndex);
	}
	else if (sign == "+")
	{
		if (_input.size() - 1 < paramIndex)
		{
			std::string flag(1, c);
			_user.printMessage(500, _input[1], flag); // ERR_CMODE
			return (paramIndex);
		}
		else if (c == 'k')
		{
			if (_ircserv.getChannel(_input[1])->getKey() == _input[paramIndex])
				return (paramIndex);
			modeKpositive(_input[paramIndex]);
		}
		else if (c == 'o') {
			modeOpositive(_input[paramIndex]);
		}
		else if (c == 'l') {
			std::string tmp = parseParamL(_input[paramIndex], c);
			if (tmp == "0")
				return (paramIndex);
			else if (tmp != _input[paramIndex])
				_input[paramIndex] = tmp;
			std::cout << _input[paramIndex] << "." << std::endl;
			modeLpositive(_input[paramIndex]);
			// if (modeLpositive(_input[paramIndex]) == false)
			// 	return (paramIndex);
		}
		paramIndex++;
	}
	else if (sign == "-")
	{
		if (_input.size() - 1 < paramIndex && c == 'o')
		{
			std::string flag(1, c);
			_user.printMessage(500, _input[1], flag); // ERR_CMODE
			return (paramIndex);
		}
		else if (c == 'k') {
			modeKnegative();
		}
		else if (c == 'o') {
			modeOnegative(_input[paramIndex]);
		}
		else if (c == 'l') {
			modeLnegative();
		}
		paramIndex++;
	}

	return (paramIndex);
}

void	Command::modeFind(Channel *channel)
{
	std::string modeKnown = "iklto";
	std::string modeInputClear;
	std::string input = _input[2];

	unsigned long	paramIndex = 3;
	std::string sign = "+";
	std::string param;
	std::cout << "lol" << std::endl;

	for (unsigned long i = 0; i < input.size(); i++)
	{
		if ((input[i] != '+' && input[i] != '-') && (input[i] != 'l' && sign == "+" && cInStr(input[i], channel->getModeCmd())))
			;
		else if ((input[i] != '+' && input[i] != '-') && (input[i] != 'o' && sign == "-" && !cInStr(input[i], channel->getModeCmd())))
			;
		else if ((input[i] != '+' && input[i] != '-') && cInStr(input[i], modeKnown) == false)
		{
			std::string s(1, input[i]);
			_user.printMessage(472, s); //ERR_UNKNOWNMODE
		}
		else
		{
			if (input[i] == '+' || input[i] == '-')
			{
				sign = input[i];
				modeInputClear += input[i];
			}
			else
			{
				unsigned long tmp = modeParseParam(sign, input[i], paramIndex);
				if (paramIndex != tmp)
				{
					modeInputClear += input[i];
					if (tmp > paramIndex)
						param = " " + _input[paramIndex++];
				}

			}

		}
	}
	if (modeInputClear.length() > 1)
	{
		param = ":" + _user.getPrefix() + " MODE " + _input[1] + " " + modeInputClear + param;
		std::map<int, User *> listUsers = channel->getUsers();
		for (std::map<int, User *>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
				it->second->printMessage(param + "\r\n");
		// _user.printMessage(param + "\r\n");
	}

}

void	Command::changeMode(void)
{
	std::string mode = "itkol";

	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}

	for (unsigned long i = 0; i != _input.size(); i++)
		std::cout << "input[" << i << "] = " << _input[i]  << "." << std::endl;

	if (_user.getStatus() != User::DELETED && !_ircserv.isUser(_input[1]))
	{
		if (_input[1].empty())
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (_ircserv.isUser(_input[1]))
			return;
		else if (!_ircserv.isChannel(_input[1]))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_ircserv.isChannel(_input[1]) && _input.size() == 2)
			_user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
		// else if (_ircserv.isChannel(_input[1]) && !_ircserv.getChannel(_input[1])->isOperator(_user))
		// 	_user.printMessage(482, _input[1]); //ERR_CHANOPRIVSNEEDED
		else
			modeFind(_ircserv.getChannel(_input[1]));
	}
}

void	Command::modeI(std::string &sign) {
	std::cout << "here" << std::endl;
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (sign == "+") {
		if (_ircserv.getChannel(_input[1])->getMode() == Channel::PUBLIC) {
			_ircserv.getChannel(_input[1])->setMode(Channel::INVITE_ONLY);
			if (!cInStr('i', _ircserv.getChannel(_input[1])->getModeCmd()))
				mode += "i";
			_ircserv.getChannel(_input[1])->setModeCmd(mode);
		}
		else if (_ircserv.getChannel(_input[1])->getMode() == Channel::INVITE_ONLY)
			;
	}
	else if (sign == "-") {
		if (_ircserv.getChannel(_input[1])->getMode() == Channel::INVITE_ONLY)
		{
			_ircserv.getChannel(_input[1])->setMode(Channel::PUBLIC);
			if (cInStr('i', _ircserv.getChannel(_input[1])->getModeCmd())) {
				size_t found = mode.find('i');
				if (found != std::string::npos)
					mode.erase(found, 1);
				_ircserv.getChannel(_input[1])->setModeCmd(mode);
			}
		}
		else if (_ircserv.getChannel(_input[1])->getMode() == Channel::PUBLIC)
			;
	}
}

void	Command::modeT(std::string &sign) {
	std::cout << "LALALLA " << sign << std::endl;
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (sign == "+") {
		if (cInStr('t', _ircserv.getChannel(_input[1])->getModeCmd()))
			;
		else {
			mode += "t";
			_ircserv.getChannel(_input[1])->setModeCmd(mode);
		}
	}
	else if (sign == "-") {
		if (cInStr('t', _ircserv.getChannel(_input[1])->getModeCmd())) {
			size_t found = mode.find('t');
			if (found != std::string::npos)
				mode.erase(found, 1);
			_ircserv.getChannel(_input[1])->setModeCmd(mode);
		}
		else if (!cInStr('t', _ircserv.getChannel(_input[1])->getModeCmd()))
			;
	}
}

void	Command::modeKpositive(std::string &param) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('k', _ircserv.getChannel(_input[1])->getModeCmd())) {
		if (_ircserv.getChannel(_input[1])->getKey() == param)
			;
		else {
			_ircserv.getChannel(_input[1])->setKey(param);
		}
	}
	else {
		mode += "k";
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->setKey(param);
	}
}

void	Command::modeKnegative(void) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('k', _ircserv.getChannel(_input[1])->getModeCmd())) {
		size_t found = mode.find('k');
		if (found != std::string::npos)
			mode.erase(found, 1);
	_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->setKey("");
	}
	else
		;
}

void	Command::modeLpositive(std::string &param) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('l', _ircserv.getChannel(_input[1])->getModeCmd())) {
		if (_ircserv.getChannel(_input[1])->getMaxUser() == static_cast<unsigned long>(atoi(param.c_str())))
			;
		else {
			_ircserv.getChannel(_input[1])->setMaxUser(atoi(param.c_str()));
		}
	}
	else {
		mode += "l";
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->setMaxUser(atoi(param.c_str()));
	}
}

void	Command::modeLnegative(void) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('l', _ircserv.getChannel(_input[1])->getModeCmd())) {
		size_t found = mode.find('l');
		if (found != std::string::npos)
			mode.erase(found, 1);
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->setMaxUser(0);
	}
	else
		;
}

void	Command::modeOpositive(std::string &param) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('o', _ircserv.getChannel(_input[1])->getModeCmd())) {
		if (_ircserv.getChannel(_input[1])->isOperator(param))
			;
		else {
			_ircserv.getChannel(_input[1])->addOperator(*_ircserv.getUser(param));
		}
	}
	else {
		mode += "o";
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->addOperator(*_ircserv.getUser(param));
	}
}

void	Command::modeOnegative(std::string &param) {
	std::string mode = _ircserv.getChannel(_input[1])->getModeCmd();
	if (cInStr('o', _ircserv.getChannel(_input[1])->getModeCmd())) {
		size_t found = mode.find('o');
		if (found != std::string::npos)
			mode.erase(found, 1);
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->removeOperator(param);
	}
	else
		;
}
