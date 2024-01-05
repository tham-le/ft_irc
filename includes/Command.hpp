#ifndef COMMAND_HPP
#define COMMAND_HPP


#include <string>
#include <map>
#include <vector>

#include "User.hpp"
#include "Config.hpp"

class User;
class Ircserv;





class Command
{
public:
	Command();
	Command(User *user, std::string command, std::string message);

	~Command();

	void					setUser(User *user);
	User					*getUser() const;

	Ircserv					*getIrcserv() const;

private:
	std::string _command;
	std::string _prefix;
	std::vector<std::string> _params;
};

#endif
