#include "../includes/User.hpp"
#include "../includes/Color.h"
#include <unistd.h>

void	User::printMessage(std::string str) {
	std::cout <<BOLD  GREEN << ">>> " RESET<< str;
	if (write(_fd, str.c_str(), str.size()) < 0)
		throw std::runtime_error("write() failed");
}

std::string	User::getPrefix() const
{
	if (_status == PASSWORD_REQUIRED)
		return ("");
	std::string prefix = _nickname;
	if (_hostname != "")
	{
		if (_username != "")
			prefix += "!" + _username + "@" + _hostname;
		else
			prefix += "!" + _hostname;
	}
	else
		prefix += "!" + _username;
	return (prefix);
}

static std::string	convert3digits(int n)
{
	std::string str = to_string(n);
	while (str.length() < 3)
		str = "0" + str;
	return (str);
}

 std::string	User::getTarget(int code)
{
	std::string str = ":";
	str += getPrefix();
	str += " ";
	str += convert3digits(code);
	str += " ";
	if (_nickname != "")
		str += _nickname;
	else
		str += "*";
	str += " ";
	return (str);
}


void	User::printMessage(int code)
{
	std::string str = getTarget(code);

	switch (code) {
	case 001:
		str += RPL_WELCOME(_nickname);
		break;
	case 002:
		str += RPL_YOURHOST(_ircserv->getHostName(), _ircserv->getVersion());
		break;
	case 003:
		str += RPL_CREATED(_ircserv->getStartTime());
		break;
	case 004:
		str += RPL_MYINFO(_ircserv->getHostName(), _ircserv->getVersion(), "ao", "mtov");
		break;
	case 005:
		str += RPL_ISUPPORT();
		break;
	case 311:
		str += RPL_WHOISUSER(_nickname, _username, _hostname, _realname);
		break;
	case 312:
		str += RPL_WHOISSERVER(_nickname, _ircserv->getHostName(), _ircserv->getVersion());
		break;
	case 319:
		str += RPL_WHOISCHANNELS(_nickname, getNameChannels());
		break;
	case 321:
		str += RPL_LISTSTART();
		break;
	case 322:
		str += RPL_LIST(getLastChannel()->getName(), getLastChannel()->getUsersName(), getLastChannel()->getTopic());
		break;
	case 323:
		str += RPL_LISTEND();
		break;
	case 431:
		str += ERR_NONICKNAMEGIVEN();
		break;
	case 462:
		str += ERR_ALREADYREGISTRED();
		break;
	case 451:
		str += ERR_NOTREGISTERED();
		break;
	}
	printMessage(str);
}




void	User::printMessage(int code, std::string m1)
{
	std::string str = getTarget(code);

	switch (code) {
	case 331:
		str += RPL_NOTOPIC(m1);
		break;
	case 366:
		str += RPL_ENDOFNAMES(m1);
		break;
	case 403:
		str += ERR_NOSUCHCHANNEL(m1);
		break;
	case 405:
		str += ERR_TOOMANYCHANNELS(m1);
		break;
	case 421:
		str += ERR_UNKNOWNCOMMAND(m1);
		break;
	case 432:
		str += ERR_ERRONEUSNICKNAME(m1);
		break;
	case 442:
		str += ERR_NOTONCHANNEL(m1);
		break;
	case 461:
		str += ERR_NEEDMOREPARAMS(m1);
		break;
	case 471:
		str += ERR_CHANNELISFULL(m1);
		break;
	case 473:
		str += ERR_INVITEONLYCHAN(m1);
		break;
	case 475:
		str += ERR_BADCHANNELKEY(m1);
		break;
	case 482:
		str += ERR_CHANOPRIVSNEEDED(m1);
		break;
	}
	printMessage(str);

}

void	User::printMessage(int code, std::string m1, std::string m2)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 332:
		str += RPL_TOPIC(m1, m2);
		std::cout << "caca prout" << m1 << " " << m2 << std::endl;
		break;
	case 333:
		str += RPL_TOPICWHOTIME(m1, m2);
		break;
	case 353:
		str += RPL_NAMREPLY(m1, m2);
		break;

	}
	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {

	case 322:
		str += RPL_LIST(m1, m2, m3); //EXAMPLE, DONT USE
		break;
	}
	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 319:
		str += RPL_WHOISCHANNELS(m1, m2); //EXAMPLE, DONT USE
		break;
	case 366:
		str += RPL_ENDOFNAMES(m3); //EXAMPLE, DONT USE
		break;
	}
	(void)m4;


	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 353:
		str += RPL_NAMREPLY(m1, m2); //EXAMPLE, DONT USE
		break;
	}
	(void)m3;
	(void)m4;
	(void)m5;

	printMessage(str);
}

void User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 311:
		str += RPL_WHOISUSER(m1, m2, m3, m4); //EXAMPLE, DONT USE
		break;
	}
	(void)m5;
	(void)m6;

	printMessage(str);
}


void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6, std::string m7)
{
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 311:
		str += RPL_WHOISUSER(m1, m2, m3, m4); //EXAMPLE, DONT USE
		break;
	}
	(void)m5;
	(void)m6;
	(void)m7;

	printMessage(str);
}



