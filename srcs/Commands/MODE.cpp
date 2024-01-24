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
	(void) paramIndex;
	(void) sign;
	(void) c;
	if (c == 'i')
		std::cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << std::endl;
		// modeI(sign);
	else if (c == 't')
		std::cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << std::endl;
		// modeT(sign);
	else
	if (sign == "+")
	{
		std::cout << _input.size() << " lll" << std::endl;
		if (_input.size() - 1 < paramIndex)
		{
				std::cout << "ERRREURRRRRR avec _input size = " << _input.size() << std::endl; //message erreur
			return (paramIndex);
		}
		else if (c == 'k')
			std::cout << "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK AVEC " << _input[paramIndex] << std::endl;
				// modeKpositive(_input[paramIndex]);
		else if (c == 'o')
			std::cout << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO AVEC " << _input[paramIndex] << std::endl;
				// modeOpositive(_input[paramIndex]);
		else if (c == 'l')
			std::cout << "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL AVEC " << _input[paramIndex] << std::endl;
				// modeLpositive(_input[paramIndex]);
		paramIndex++;
	}
	else if (sign == "-")
	{
		if (_input.size() - 1 < paramIndex && c == 'o')
		{
				std::cout << "ERRREURRRRRR avec _input size = " << _input.size() << std::endl; //message erreur
			return (paramIndex);
		}
		else if (c == 'k')
			std::cout << "------------------KKKKKKKKKKKKK AVEC " << _input[paramIndex] << std::endl;
			// modeKnegative();
		else if (c == 'o')
			std::cout << "------------------OOOOOOOOOOOOOOOO AVEC " << _input[paramIndex] << std::endl;
			// modeOnegative(_input[paramIndex]);
		else if (c == 'l')
			std::cout << "------------------LLLLLLLLLLLLLLLLLLLLLL AVEC " << _input[paramIndex] << std::endl;
			// modeLnegative();
		paramIndex++;
	}

	return (paramIndex);
}

void	Command::modeFind(Channel *channel)
{
	std::string modeKnown = "iklto";
	std::string input = _input[2];
	std::string modeInputClear(1, input[0]);

	unsigned long	paramIndex = 3;
	std::string sign = "+";

	for (int i = 0; input[i]; i++)
	{
		if ((input[i] != '+' && input[i] != '-') && cInStr(input[i], channel->getModeCmd()))
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
				if (paramIndex != modeParseParam(sign, input[i], paramIndex))
				{
					if (input[i] != 'o')
						modeInputClear += input[i];
					paramIndex++;
				}

			}

		}
	}

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
			modeFind(_ircserv.getChannel(_input[1]));
	}
	else
	{
		if (_input[1].empty() || (_ircserv.isChannel(_input[1]) && _input.size() == 2))
		{
			_user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
			_user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
		}
		else
			modeFind(_ircserv.getChannel(_input[1]));
	}


}
