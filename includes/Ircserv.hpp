#ifndef IRC_SERV_HPP
#define IRC_SERV_HPP

# define MAX_CLIENTS 100
#define BUFF_SIZE 512
#define FOPEN_MAX FD_SETSIZE



#include <string>
#include <map>
#include <vector>
#include "User.hpp"
#include "Channel.hpp"
#include "Config.hpp"
#include <poll.h>
#include <csignal>

class User;
class Channel;
class Command;

class Ircserv
{
	public:
		~Ircserv();
		Ircserv(int port, std::string password);

		void			init();
		void			run();
		void			waitForEvent();
		void			connectClient();
		void			sendPendingMessages();

		void			writeToClient(int fd, std::string const &msg);
		void			writeToAllClients(std::string const &msg);
		void			writeToAllClientsExcept(int fd, std::string const &msg);

		std::string		readFromClient(int fd);
		void		readFromAllClients();

		void			putStr(std::string const &str);
		void			disconnectClient(int fd);
		void			closeAllSocket();

		void			handleMessage(User &user, std::string const &msg);

		void			putStrFd(int fd, std::string const &str);

		User			&getUser(int fd) const;

		std::string			getStartTime();

		int				getSocketFd() const;

		void			sendPing();

		void			addChannel(std::string const channel);
		bool			isChannel(std::string const channel) const;
		void			removeChannel(std::string const channel);
		Channel			*getChannel(std::string const channel);
		std::string		getChannelsName() const;
		void			disconnectAllClients();

		std::string		getHostName() const;
		std::string		getVersion() const;

/*		Config			getConfig() const;
		void			setConfig(Config const &config);

		void			addUser(User *user);
		void			removeUser(User *user);
		void			removeUser(std::string const nickname);

		std::map<std::string, Channel>	getChannels() const;*/


	private:
		friend class Command;
		std::map<int, User *>			_users;
		std::map<std::string, Channel *>	_channels;
		Config							_config;


		int 							_sockfd;
		std::vector <pollfd>			_pollfds;
		time_t							_lastPing;

		std::string						_startTime;
		bool							_stopSignal;
		std::string						_hostName;
		std::string						_version;



		class	DisconnectedUser : public std::exception
		{
			public:
				int		_fd;

				DisconnectedUser(int fd);
				~DisconnectedUser() throw();
				const char *what() const throw();
		};

};

#endif
