#include "../includes/Command.hpp"

Command::Command() : _name("") {}

Command::Command(std::string name) {
	parse(name);
}

Command::~Command() {}

std::string	Command::getName() const
{
	return _name;
}
