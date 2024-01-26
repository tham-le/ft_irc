#include "../includes/User.hpp"
#include <unistd.h>

void	User::printMessage(std::string str) {

	_ircserv->log("fd " + to_string(_fd) + "<<<<<<<< "+str);
	if (_status == DELETED)
		return ;
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
	if (_status == DELETED)
		return ;
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
		str += RPL_MYINFO(_ircserv->getHostName(), _ircserv->getVersion(), "o", "iktl");
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
	case 323:
		str += RPL_LISTEND();
		break;
	case 374:
		str += RPL_ENDOFINFO();
		break;
	case 375:
		str += RPL_MOTDSTART(_ircserv->getHostName());
		break;
	case 376:
		str += RPL_ENDOFMOTD();
		break;
	case 407:
		str += ERR_TOOMANYTARGETS();
		break;
	case 412:
		str += ERR_NOTEXTTOSEND();
		break;
	case 417:
		str += ERR_INPUTTOOLONG();
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
	case 501:
		str += ERR_UMODEUKNOWNFLAG();
		break;
	case 502:
		str += ERR_USERSDONTMATCH()
		break;
	}
	str += "\r\n";
	printMessage(str);
}

void	User::printMessage(int code, std::string m1)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);

	switch (code) {
	case 257:
		str += RPL_ADMINLOC1(m1);
		break;
	case 258:
		str += RPL_ADMINLOC2(m1);
		break;
	case 259:
		str += RPL_ADMINEMAIL(m1);
		break;
	case 331:
		str += RPL_NOTOPIC(m1);
		break;
	case 366:
		str += RPL_ENDOFNAMES(m1);
		break;
	case 372:
		str += RPL_MOTD(m1);
		break;
	case 401:
		str += ERR_NOSUCHNICK(m1);
		break;
	case 402:
		str += ERR_NOSUCHSERVER(m1);
		break;
	case 403:
		str += ERR_NOSUCHCHANNEL(m1);
		break;
	case 405:
		str += ERR_TOOMANYCHANNELS(m1);
		break;
	case 413:
		str += ERR_NOTOPLEVEL(m1);
		break;
	case 414:
		str += ERR_WILDTOPLEVEL(m1);
		break;
	case 421:
		str += ERR_UNKNOWNCOMMAND(m1);
		break;
	case 432:
		str += ERR_ERRONEUSNICKNAME(m1);
		break;
	case 433:
		str += ERR_NICKNAMEINUSE(m1);
		break;
	case 436:
		str += ERR_NICKCOLLISION(m1);
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
	case 472:
		str += ERR_UNKNOWNMODE(m1);
		break;
	case 473:
		str += ERR_INVITEONLYCHAN(m1);
		break;
	case 475:
		str += ERR_BADCHANNELKEY(m1);
		break;
	case 476:
		str += ERR_BADCHANMASK(m1);
		break;
	case 482:
		str += ERR_CHANOPRIVSNEEDED(m1);
		break;
	}
	str += "\r\n";
	printMessage(str);

}

void	User::printMessage(int code, std::string m1, std::string m2)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);;

	switch (code) {
	case 256:
		str += RPL_ADMINME(m1, m2);
		break;
	case 324:
		str += RPL_CHANNELMODEIS(m1, m2);
		break;
	case 332:
		str += RPL_TOPIC(m1, m2);
		break;
	case 329:
		str += RPL_CREATIONTIME(m1, m2);
		break;
	case 333:
		str += RPL_TOPICWHOTIME(m1, m2);
		break;
	case 341:
		str += RPL_INVITING(m1, m2);
		break;
	case 351:
		str += RPL_VERSION(m1, m2);
		break;
	case 353:
		str += RPL_NAMREPLY(m1, m2);
		break;
	case 371:
		str += RPL_INFO(m1, m2);
		break;
	case 441:
		str += ERR_USERNOTINCHANNEL(m1, m2);
		break;
	case 443:
		str += ERR_USERONCHANNEL(m1, m2);
		break;
	case 500:
		str += ERR_CMODE(m1, m2);
		break;
	}
	str += "\r\n";
	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);;

	switch (code) {

	case 319:
		str += RPL_WHOISCHANNELS(m1, m2); //EXAMPLE, DONT USE
		break;
	case 366:
		str += RPL_ENDOFNAMES(m3); //EXAMPLE, DONT USE
		break;
	}
	str += "\r\n";
	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);;

	switch (code) {
	case 319:
		str += RPL_WHOISCHANNELS(m1, m2); //EXAMPLE, DONT USE
		break;
	case 366:
		str += RPL_ENDOFNAMES(m3); //EXAMPLE, DONT USE
		break;
	}
	(void)m4;

	str += "\r\n";
	printMessage(str);
}

void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);;

	switch (code) {
	case 353:
		str += RPL_NAMREPLY(m1, m2); //EXAMPLE, DONT USE
		break;
	}
	(void)m3;
	(void)m4;
	(void)m5;
	str += "\r\n";
	printMessage(str);
}

void User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6)
{
	if (_status == DELETED)
		return ;
	std::string str = getTarget(code);;
	str += " ";

	switch (code) {
	case 311:
		str += RPL_WHOISUSER(m1, m2, m3, m4); //EXAMPLE, DONT USE
		break;
	}
	(void)m5;
	(void)m6;
	str += "\r\n";
	printMessage(str);
}


void	User::printMessage(int code, std::string m1, std::string m2, std::string m3, std::string m4, std::string m5, std::string m6, std::string m7)
{
	if (_status == DELETED)
		return ;
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

	str += "\r\n";
	printMessage(str);
}



