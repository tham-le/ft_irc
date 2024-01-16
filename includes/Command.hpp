#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
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
	void		parse(std::string msg);
	// std::string	getName() const;

	void		admin(std::string const &msg);//Y
	void		info(std::string const &msg); //Y
	void		join(std::string const &channel);//C
	void		nickname(std::string const &msg);//Y
	void		names(std::string const &channel); //Y
	void		part(std::string const &channel);
	void		quit(std::string const &msg); //Y
	void 		list(std::string const &channel); //Y
	void		kick(std::string const &channel);//C
	void		invite(std::string const &nickname, std::string const &channel);//C
	void		topic(std::string const &msg);//C
	void		changeMode(std::string const &msg);
	bool		isInChannel(std::string const &channel);
	void 		version(std::string const &msg);

	void		split(std::string str, char separator);
	void		command();
	void		initCmd();
	bool		validNickname(std::string const nickname) const;
	bool		isCmdNoUse(std::string const str) const;



private:
	std::string _msg;
	User &_user;
	Ircserv &_ircserv;
	// enum e_type
	// {
	// 	MSG;
	// 	CMD;
	// };
	// e_type _type;
	std::vector<std::string> _input;
	typedef void (Command::*FuncType)(std::string const &);
	std::map<std::string, FuncType > _func;
	// std::map<std::string, Channel &> _channels;
	// std::vector<Channel *> _channels;
	std::vector<Channel *> _lastChannels;
	std::map<std::string, Channel *>	_channels;
	std::vector<std::string> _noFunctionalOnChannel; //cmd qui ne fonctionne pas lorsque nous sommes sur un channel
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
	for (typename std::vector<T>::const_itearot it = v.begin(); it != v.end(); it++) {
		os << *it << ' ';
	}
	return os;
}

#endif
