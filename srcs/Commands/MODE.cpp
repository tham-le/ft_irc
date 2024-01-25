#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

bool	Command::cInStr(char c, std::string str)
{
	size_t found = str.find(c);
	if (found != std::string::npos)
		return (true);
	return (false);
}

unsigned long	Command::modeParseParam(std::string sign, char c, unsigned long paramIndex)
{
	if (c == 'i')
	{
		std::cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << std::endl;
		modeI(sign);
		return (--paramIndex);
	}
	else if (c == 't')
	{
		std::cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << std::endl;
		modeT(sign);
		return (--paramIndex);
	}
	else
	if (sign == "+")
	{
		std::cout << _input.size() << " lll" << std::endl;
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
			std::cout << "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK AVEC " << _input[paramIndex] << std::endl;
			modeKpositive(_input[paramIndex]);
		}
		else if (c == 'o') {
			std::cout << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO AVEC " << _input[paramIndex] << std::endl;
			modeOpositive(_input[paramIndex]);
		}
		else if (c == 'l') {
			std::cout << "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL AVEC " << _input[paramIndex] << std::endl;
			modeLpositive(_input[paramIndex]);
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
			std::cout << "------------------KKKKKKKKKKKKK AVEC " << _input[paramIndex] << std::endl;
			modeKnegative();
		}
		else if (c == 'o') {
			std::cout << "------------------OOOOOOOOOOOOOOOO AVEC " << _input[paramIndex] << std::endl;
			modeOnegative(_input[paramIndex]);
		}
		else if (c == 'l') {
			std::cout << "------------------LLLLLLLLLLLLLLLLLLLLLL AVEC " << _input[paramIndex] << std::endl;
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

	unsigned long	paramIndex = 3;
	std::string sign = "+";
	std::string param;

	for (unsigned long i = 0; i < _input[2].size(); i++)
	{
		if ((_input[2][i] != '+' && _input[2][i] != '-') && cInStr(_input[2][i], channel->getModeCmd()))
			;
		else if ((_input[2][i] != '+' && _input[2][i] != '-') && cInStr(_input[2][i], modeKnown) == false)
		{
			std::string s(1, _input[2][i]);
			_user.printMessage(472, s); //ERR_UNKNOWNMODE
		}
		else
		{
			if (_input[2][i] == '+' || _input[2][i] == '-')
			{
				sign = _input[2][i];
				modeInputClear += _input[2][i];
			}
			else
			{
				unsigned long tmp = modeParseParam(sign, _input[2][i], paramIndex);
				if (paramIndex != tmp)
				{
					modeInputClear += _input[2][i];
					if (tmp > paramIndex)
						param = " " + _input[paramIndex++];
				}

			}

		}
	}
	if (modeInputClear.length() > 1)
	{
		param = ":" + _user.getPrefix() + " MODE " + _input[1] + " " + modeInputClear + param;
		std::cout << param << "!" <<std::endl;
		_user.printMessage(param + "\r\n");
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
		if (_ircserv.getChannel(_input[1])->getMode() == Channel::INVITE_ONLY) {
			_ircserv.getChannel(_input[1])->setMode(Channel::PUBLIC);
			if (cInStr('i', _ircserv.getChannel(_input[1])->getModeCmd()))
				mode.erase('i', 1);
			_ircserv.getChannel(_input[1])->setModeCmd(mode);
		}
		else
			;
	}
}

void	Command::modeT(std::string &sign) {
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
			mode.erase('t', 1);
			_ircserv.getChannel(_input[1])->setModeCmd(mode);
		}
		else
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
		mode.erase('k', 1);
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
		mode.erase('l', 1);
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
		mode.erase('o', 1);
		_ircserv.getChannel(_input[1])->setModeCmd(mode);
		_ircserv.getChannel(_input[1])->removeOperator(param);
	}
	else
		;
}
