#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::topicChannel(Channel *channel, int idx)
{
	std::string msg;
	std::string	topic = _input[idx];
	for (unsigned long i = idx + 1; i < _input.size(); i++)
		topic += " " + _input[i];
	channel->setTopic(topic, _user.getNickname());
	msg = ":" + _user.getPrefix() + " TOPIC " + channel->getName() + " " + topic + "\r\n";
	_user.printMessage(msg);
	_user.printMessage(332, channel->getName(), topic); //  RPL_TOPIC
	_user.printMessage(333, _user.getNickname(), channel->getTopicTime()); // RPL_TOPICWHOTIME

}

void		Command::topicCheck(Channel *channel, int idx)
{
	(void)idx;
	// channel = _ircserv.getChannel(_input[1]);
	std::cout << "PIPI1 " << _input[1] << std::endl;
	std::cout << "PIPI2 " << _input[2] << std::endl;
	std::cout << "FIN" << std::endl;
	if (!_ircserv.isChannel(_input[1]))
		_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
	else if (!channel->isUserInChannel(_user))
		_user.printMessage(442, _input[1]); //ERR_NOTONCHANNEL
	else if (!_input[2].empty() && !channel->isOperator(_user))
		_user.printMessage(482, channel->getName());// ERR_CHANOPRIVSNEEDED
	else if (!_input[2].empty() && channel->isOperator(_user))
		topicChannel(channel, 2);
	else
	{
		_user.printMessage(332, channel->getName(), channel->getTopic()); //  RPL_TOPIC
		_user.printMessage(333, _user.getNickname(), channel->getTopicTime()); // RPL_TOPICWHOTIME
	}
}


void		Command::topic()
{
	if (_user.getStatus() == User::ONLINE)
	{
		if (_input.size() > 2)
		{
			for (unsigned long i = 0; i != _input.size(); i++)
				std::cout << "input[" << i << "] = " << _input[i] <<std::endl;
			topicCheck(_ircserv.getChannel(_input[1]), 2);
		}
	}
	else
	{
		for (unsigned long i = 0; i != _input.size(); i++)
				std::cout << "input[" << i << "] = " << _input[i] <<std::endl;
		if (_input[1].empty())
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (_input[1][0] != '#' || (_input[1][0] == '#' && !_ircserv.isChannel(&_input[1][1])))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_input[1][0] == '#' && _ircserv.isChannel(&_input[1][1]))
			topicCheck(_ircserv.getChannel(&_input[1][1]), 2);
	}
}
