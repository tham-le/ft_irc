#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <exception>


#include "User.hpp"
#include "Ircserv.hpp"
#include "Channel.hpp"

class User;
class Ircserv;
class Channel;

class Command
{
public:

	Command(std::string const &msg, User &user, Ircserv &ircserv);

	~Command();

	void		run(int fd, Ircserv &ircserv);
	// std::string	getName() const;


	void		pass(void);
	void		cap(void);


	void		who(void);
	void		ping(void);
	void		pong(void);
	void		privmsg(void);
	void		notice(void);




	void		admin(void);
	void		info(void); //Y
	void		join(void);//C
	void		nickname(void);//Y
	void		names(void); //Y
	void		part(void);
	void		quit(void); //Y
	void 		list(void); //Y
	void		kick(void);//C
	void		invite(void);//C
	void		topic(void);//C
	void		changeMode(void);
	bool		isInChannel(void);
	void 		version(void);
	void		user(void);
	void		whois(void);

	std::vector<std::string>	split(std::string str, char separator);
	void		command();
	void		initCmd();
	bool		validNickname(std::string const nickname) const;
	// bool		isCmdNoUse(std::string const str) const;
	std::string		toFormat(std::string cmd, std::string str);
	// std::string		toChannelName(std::string str);
	void		joinChannel(Channel *channel);
	void		topicChannel(Channel *channel);
	void		topicCheck(Channel *channel);


	/*a supprimer , pour tester*/
	void		channelUsers(std::string channel); //print la liste des users du channel
	/**/

private:
	User						&_user;
	Ircserv						&_ircserv;
	std::vector<std::string>	_input;
	typedef void 				(Command::*FuncType)();
	std::map<std::string, FuncType >		_func;
	std::vector<Channel *>					_lastChannels;
	std::map<std::string, Channel *>		_channels;
};

/*Template to convert int to string*/
template<typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

/* Template to print vector. Usage: std::cout << myVector << std::endl;*/
template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T> &v) {
	for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); it++) {
		os << *it << ' ';
	}
	return os;
}

#endif
