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
	Command(std::string message);

	~Command();

	void		run(int fd, Ircserv &ircserv);
	void		parse(std::string message);
	std::string	getName() const;

private:
	std::string _name;
};

#endif
