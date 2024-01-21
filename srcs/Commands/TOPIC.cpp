#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::topicChannel(Channel *channel, int idx)
{
	std::string msg;
	std::string	topic = _input[idx];
	for (unsigned long i = idx + 1; i < _input.size(); i++)
		topic += " " + _input[i];
	channel->setTopic(topic, _user.getNickname());
	msg = ":" + _user.getPrefix() + " TOPIC #" + channel->getName() + " :" + topic + "\r\n";
	_user.printMessage(msg);
	_user.printMessage(332, channel->getName(), topic); //  RPL_TOPIC
	_user.printMessage(333, _user.getNickname(), channel->getTopicTime()); // RPL_TOPICWHOTIME

}

void		Command::topicCheck(Channel *channel, int idx)
{
	if (!channel->isUserInChannel(_user))
		_user.printMessage(442, channel->getName()); //ERR_NOTONCHANNEL
	else if (!_input[idx].empty() && !channel->isOperator(_user))
		_user.printMessage(482, channel->getName());// ERR_CHANOPRIVSNEEDED
	else if (!_input[idx].empty() && channel->isOperator(_user))
		topicChannel(channel, idx);
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
			if (_input[1][0] != '#')
				topicChannel(_ircserv.getChannel(&_input[1][1]), 1);
			else
				topicCheck(_ircserv.getChannel(&_input[1][1]), 2);
		}
	}
	else
	{
		if (_input[1].empty())
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (_input[1][0] != '#' || (_input[1][0] == '#' && !_ircserv.isChannel(&_input[1][1])))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_input[1][0] == '#' && _ircserv.isChannel(&_input[1][1]))
			topicCheck(_ircserv.getChannel(&_input[1][1]), 2);
	}
}
