#include <iostream>
#include <exception>
#include <signal.h>
#include "../includes/sillyBot.hpp"
#include <stdlib.h>
#include "unistd.h"

bool alive = true;

static void handler(int sig)
{
	(void)sig;
	alive = false;
}

static int toInt(char const *port)
{
	int i = 0;
	while (port[i])
	{
		if (!std::isdigit(port[i]))
			throw std::invalid_argument("Port must be a number");
		i++;
	}
	return (atoi(port));
}


int main(int argc, char const *argv[])
{
	try {
		if (argc != 5)
			throw std::invalid_argument("Usage: ./sillyBot <host> <port> <password> <config_file>");
		int port = toInt(argv[2]);

		signal(SIGINT, handler);
		signal(SIGPIPE, SIG_IGN);

		sillyBot bot(argv[1], port, argv[3], argv[4]);
		while (alive)
			bot.run();
		
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return 0;
}

