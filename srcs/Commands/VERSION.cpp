#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void	Command::version(void)
{
	for (unsigned long i = 0; i != _input.size(); i++)
		std::cout << "input[" << i << "] = " << _input[i] << std::endl;
}
