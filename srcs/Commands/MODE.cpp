#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

bool	Command::cInStr(char c, std::string str)
{
	size_t found = str.find(c);
	if (found != std::string::npos)
		return (true);
	return (false);
}

void	Command::modeParseParam(std::string modeToChange, Channel *channel)
{
	// int		start;
	char	sign = modeToChange[0];

	for (int i = 0; modeToChange[i]; i++)
	{
		if (modeToChange[i] == 'i')
		{
			if (sign == '+')
				channel->setMode(Channel::INVITE_ONLY);
			else
				channel->setMode(Channel::PUBLIC);
		}
		// else if (modeToChange[i] == 'k')
		// {
		// 	if (key )
		// }
		// else if (modeToChange[i] == 'l')
		// else if (modeToChange[i] == 't')
		// else if (modeToChange[i] == 'o')
		// else
		// 	sign = modeToChange[i];

	}
}

void	Command::modeFind(std::string modeInput, Channel *channel)
{
	std::string modeKnown = "iklto";
	std::string	modeInputClear;

	for (int i = 0; modeInput[i]; i++)
	{
		if ((modeInput[i] != '+' && modeInput[i] != '-') && cInStr(modeInput[i], channel->getModeCmd()))
			;
		else if ((modeInput[i] != '+' && modeInput[i] != '-') && cInStr(modeInput[i], modeKnown) == false)
		{
			std::string s(1, modeInput[i]);
			_user.printMessage(472, s); //ERR_UNKNOWNMODE
		}
		else
		{
			modeInputClear += modeInput[i];
		}
	}

	std::cout << modeInputClear << "!" << std::endl;
	if (!modeInputClear.empty())
	{
		modeParseParam(modeInputClear, channel);
	}
	//  for (int i = 0; modeInput[i]; i++)
	//  {

	//  }
}


void	Command::changeMode(void)
{
	std::string mode = "itkol";

	for (unsigned long i = 0; i != _input.size(); i++)
		std::cout << "input[" << i << "] = " << _input[i]  << "." << std::endl;

	if (_user.getStatus() != User::DELETED)
	{
		if (_input[1].empty())
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (_ircserv.isUser(_input[1]))
			return;
		else if (!_ircserv.isChannel(_input[1]))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_ircserv.isChannel(_input[1]) && _input.size() == 2)
		{
			_user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
			_user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
		}
		// else if (_ircserv.isChannel(_input[1]) && !_ircserv.getChannel(_input[1])->isOperator(_user))
		// 	_user.printMessage(482, _input[1]); //ERR_CHANOPRIVSNEEDED
		else
			modeFind(_input[2], _ircserv.getChannel(_input[1]));
	}
	// else
	// {
	// 	if (_input[1].empty() || (_ircserv.isChannel(_input[1]) && _input.size() == 2))
	// 	{
	// 		// _user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
	// 		// _user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
	// 	}
	// 	// else
	// 	// 	modeFind(_ircserv.getChannel(&_input[1][1])->getModeCmd(), _input[2], _ircserv.getChannel(&_input[1][1]));
	// }


}
