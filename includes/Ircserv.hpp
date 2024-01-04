#ifndef IRC_SERV_HPP
#define IRC_SERV_HPP

#include <string>
#include <map>
#include "Users.hpp"
#include <Channel.hpp>


class Ircserv
{
	public:
		Ircserv(int port, std::string password);
		Ircserv(Ircserv const &src);
		Ircserv &operator=(Ircserv const &rhs);

		~Ircserv();
		void start(void);
		void stop(void);
		void handleMessage(std::string message);
	private:
		int _serverSocket;
		int _port;

		std::map<int, User *> users;
		std::map<std::string, Channel> channels;

};

#endif
