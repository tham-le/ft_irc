#ifndef USER_HPP
#define USER_HPP


#include <string>
#include <map>
#include <vector>

class Users
{
private:
	std::map<std::string, void (*)(Command *)> command_function;

	std::string	_nickname;
	std::string	_username;
	std::string	_realname;
	std::string	_hostname;
	std::string	_servername;
	std::string	_password;
	std::string	_ip;

	bool		_isOperator;
	bool		_isAway;
	std::string awayMessage;
	
	std::vector<std::string> _channels;
		
}

#endif
