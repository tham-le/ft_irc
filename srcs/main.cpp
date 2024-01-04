/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-le <thi-le@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 20:50:16 by thi-le            #+#    #+#             */
/*   Updated: 2024/01/04 20:42:29 by thi-le           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <csignal>
#include <Ircserv.hpp>

bool stop = false;

void signal_handler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	stop = true;
}



int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::invalid_argument("Usage: ./ircserv <port> <password>");
		int port = atoi(argv[1]); 
		std::string password = argv[2];

		IrcServ ircserv(port, password);
		signal(SIGINT, signal_handler);

		while (!stop)
			ircserv.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
