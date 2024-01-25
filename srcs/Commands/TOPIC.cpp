#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::topicChannel(Channel *channel)
{
	std::string msg;
	std::string	topic = _input[2];
	for (unsigned long i = 3; i < _input.size(); i++)
		topic += " " + _input[i];
	if (topic.length() > 307)
	{
		_user.printMessage("Topic too long, truncated to 307 characters");
		topic = topic.substr(0, 307);
	}
	channel->setTopic(topic, _user.getNickname());
	msg = ":" + _user.getPrefix() + " TOPIC " + channel->getName() + " " + topic + "\r\n";
	_user.printMessage(msg);
}

void		Command::topicCheck(Channel *channel)
{
	if (!_ircserv.isChannel(_input[1]))
		_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
	else if (!channel->isUserInChannel(_user))
		_user.printMessage(442, _input[1]); //ERR_NOTONCHANNEL
	else if (!_input[2].empty() && !channel->isOperator(_user))
		_user.printMessage(482, channel->getName());// ERR_CHANOPRIVSNEEDED
	else if (!_input[2].empty() && ((channel->isOperator(_user) && Command::cInStr('t', channel->getModeCmd())) || !Command::cInStr('t', channel->getModeCmd())))
		topicChannel(channel);
	else
	{
		_user.printMessage(332, channel->getName(), channel->getTopic()); //  RPL_TOPIC
		_user.printMessage(333, _user.getNickname(), channel->getTopicTime()); // RPL_TOPICWHOTIME
	}
}


void		Command::topic()
{
	if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
	{
		_user.printMessage(451); //ERR_NOTREGISTEREd
		return ;
	}

	if (_user.getStatus() == User::ONLINE)
	{
		for (unsigned long i = 0; i != _input.size(); i++)
			std::cout << "input[" << i << "] = " << _input[i] <<std::endl;
		if (_input.size() > 2)
			topicCheck(_ircserv.getChannel(_input[1]));
	}
	else
	{
		std::cout << _input.size() << "." << std::endl;
		if (_input[1] == "")
			_user.printMessage(461, _input[0]); //ERR_NEEDMOREPARAMS
		else if (_input[1][0] != '#' || (_input[1][0] == '#' && !_ircserv.isChannel(_input[1])))
			_user.printMessage(403, _input[1]); //ERR_NOSUCHCHANNEL
		else if (_input[1][0] == '#' && _ircserv.isChannel(_input[1]))
			topicCheck(_ircserv.getChannel(_input[1]));
	}
}
