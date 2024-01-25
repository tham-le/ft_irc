/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-le <thi-le@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 20:50:16 by thi-le            #+#    #+#             */
/*   Updated: 2024/01/25 13:05:40 by thi-le           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>

#include <cstdlib>
#include <Ircserv.hpp>

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

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::invalid_argument("Usage: ./ircserv <port> <password>");
		int port = toInt(argv[1]);
		std::string password = argv[2];
	
		Ircserv ircserv(port, password);
		ircserv.init();
		ircserv.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
