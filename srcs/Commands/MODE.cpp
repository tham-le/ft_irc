#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::modeFind(std::string modeCmd, std::string mode, Channel *channel)
{
	// std::string sign;
	// std::string modeParam = ""
	std::string modeKnown = "itkol";
	std::string modeToChange = "+";

	for (unsigned int i = 0; i < mode.length(); i++)
	{
		size_t found = modeKnown.find(mode[i]);
		if (found != std::string::npos)
			modeToChange = mode[i];
		else if (mode[i] == '-' || mode[i] == '+')
			modeToChange = mode[i];
		else
			_user.printMessage(472, std::string(1, mode[i])); // ERR_UNKNOWNMODE

	}
	std ::cout << modeToChange << " lala" << std::endl;

	for (int i = 0; modeToChange[i]; i++)
	{
		if (modeToChange[i] == '+')
		{
			for (unsigned int j = i + 1; j < modeToChange.size() && isalpha(modeToChange[j]); j++)
			{
				size_t found = modeCmd.find(modeToChange[j]);
				if (found == std::string::npos)
				{
					channel->setModeCmd(modeCmd + modeToChange[j]);
					std::cout << channel->getModeCmd() << ".." << std::endl;
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
	// std::cout << _input.size() << " !" << std::endl;
	// if (_input[2].empty())
	// {
	// 	std::cout << _user.getNickname() << " !" << std::endl;
	// 	std::cout << _user.getStatus() << " !" << std::endl;
	// }
	if (_user.getStatus() != User::ONLINE)
	{
		// std::cout << "atchoum" << std::endl;
		if (_input[1].empty())
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (!_ircserv.isChannel(_input[1]))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_ircserv.isChannel(_input[1]) && _input.size() == 2)
		{
			_user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
			_user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
		}
		else if (_ircserv.isChannel(_input[1]) && !_ircserv.getChannel(_input[1])->isOperator(_user))
			_user.printMessage(482, _input[1]); //ERR_CHANOPRIVSNEEDED
	}
	else
	{
		if (_input[1].empty() || (_ircserv.isChannel(_input[1]) && _input.size() == 2))
		{
			std::cout << "heyhey" << std::endl;
			// _user.printMessage(324, _input[1], _ircserv.getChannel(_input[1])->getModeCmd()); // RPL_CHANNELMODEIS
			// _user.printMessage(329, _input[1], _ircserv.getChannel(_input[1])->getCreationTime()); // RPL_CREATIONTIME
		}
		// else
		// 	modeFind(_ircserv.getChannel(&_input[1][1])->getModeCmd(), _input[2], _ircserv.getChannel(&_input[1][1]));
	}


	// else if ()
	// else if (!_ircserv.isUser(_input[1]))
	// 	_user.printMessage(401, _input[1]); // ERRNOSUCHNICK
	// else if (_ircserv.getUser(_input[1])->getNickname() != _user.getNickname())
	// 	_user.printMessage(502); //ERR_USERSDONTMATCH

	// if (_input[0].empty() || _input[0].size() < 2) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NEEDMOREPARAMS(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }
	// int idx = 0;
	// if (_input[0][0] == '#')
	// 	idx = 1;
	// std::map<std::string, Channel *>::iterator it = _channels.find(&_input[0][idx]);
	// if (it == _channels.end()) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(_input[0])); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }
	// if (_user.getStatus() != User::ONLINE && (_input[0][0] == '-' || _input[0][0] == '+')) {
	// 	_ircserv.writeToClient(_user.getFd(), ERR_NOTJOINEDANYCHANNEL()); //_input[0] sent to user but not on _input[0] if the user is on a _input[0]
	// 	return ;
	// }

}
