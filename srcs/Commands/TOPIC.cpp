#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"



void		Command::topic()
{
	int idx = 0;


	if (_input[1][idx] == '#')
		idx = 1;
	if (_user.getStatus() == User::ONLINE)
	{
		std::cout << "UUUUUUUUUUUUUUUU" << std::endl;
		if (_input[1].empty())
		{
			_user.getLastChannel()->setTopic("coucou", "");
			std::cout << _user.getLastChannel()->getTopic() << std::endl;
			if (_user.getLastChannel()->getTopic() == "")
				_user.printMessage(331, _user.getLastChannel()->getName());
			else if (_user.getLastChannel()->getTopic() != "")
			{
				std::cout << "hey" << std::endl;
				_user.printMessage(332, _user.getLastChannel()->getName(), _user.getLastChannel()->getTopic());
				_user.printMessage(333, _user.getNickname(), _user.getLastChannel()->getTopicTime());
			}
		}
		// else
		// {
		// 	if (idx == 1)
		// 	{
		// 		if (_ircserv.isChannel(&msg[idx]))
		// 		{
		// 			//  _user.isOperator() && _input.size() > 2)
		// 			_ircserv.getChannel(&msg[idx])->setTopic(&msg[idx]);
		// 		}
		// 		else
		// 			_ircserv.writeToClient(_user.getFd(), ERR_NOSUCHCHANNEL(std::string(&msg[idx])));
		// 	}
		// 	else
		// 	{
		// 		if (_user.isOperator())
		// 			_user.getLastChannel()->setTopic(&msg[idx]);

		// 	}
		// }
	}
}
