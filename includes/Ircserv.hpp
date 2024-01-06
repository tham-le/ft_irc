#ifndef IRC_SERV_HPP
#define IRC_SERV_HPP

#include <string>
#include <map>
#include <vector>
#include "Users.hpp"
#include "Channel.hpp"
#include "Config.hpp"
#include <poll.h>




class Ircserv
{
	public:
		Ircserv();
		~Ircserv();

		Ircserv(int port, std::string password);

		void			init();
		void			run();

		void			acceptUser();

		Config			getConfig() const;
		void			setConfig(Config const &config);

		void			addUser(User *user);
		void			removeUser(User *user);
		void			removeUser(std::string const nickname);

		bool			isChannel(std::string const channel) const;
		void			addChannel(std::string const channel);
		void			removeChannel(std::string const channel);
		Channel			*getChannel(std::string const channel);
		std::map<std::string, Channel>	getChannels() const;





	private:

		std::map<int, User *>			users;
		std::map<std::string, Channel>	channels;
		Config							_config;


		int 							_fd;
		std::vector <pollfd>			_pollfds;
		time_t							_lastPing;
};

#endif
