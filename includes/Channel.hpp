#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <string>
#include <map>
#include "User.hpp"
#include <algorithm>
#include <iterator>

class User;

class Channel
{
	public:

		typedef enum eChannelMode
		{
			INVITE_ONLY,
			PUBLIC,
			PROTECTED
		} t_channelMode;
		// Channel();
		Channel(std::string name);
		~Channel();

		void			setName(std::string const name);
		std::string		getName() const;

		void			setTopic(std::string const topic);
		std::string		getTopic() const;

		void			setMode(t_channelMode mode);
		// enum eChannelMode	getMode() const;

		void			setKey(std::string const key);
		std::string		getKey() const;

		void			setMaxUser(int maxUser);
		int				getMaxUser() const;


		void			addUser(User &user);
		void			removeUser(User &user);
		void			removeUser(std::string const nickname);
		User*			getUser(std::string const nickname);
		std::map<int, User *>	getUsers();


		bool			isUserInChannel(std::string const nickname) const;
		bool			isUserInChannel(User const &user) const;

		// void			Ban(User &user);
		// void			Unban(User &user);
		// void			Unban(std::string const nickname);
		// bool			isBanned(User &user);

		void			Invite(User &user);
		void			Uninvite(User &user);
		void			Uninvite(std::string const nickname);
		bool			isInvited(User &user);

		bool			isGoodKey(std::string key);

		void			addOperator(User &user);
		void			removeOperator(User &user);
		void			removeOperator(std::string const nickname);
		bool			isOperator(User &user);
		bool			isOperator(std::string const nickname);
		bool			isEmptyOperator();

private:
		std::string		_name;
		std::string		_topic;
		t_channelMode	_mode;
		std::string		_key;

		unsigned long			_maxUser;

		std::map<int, User *>	_users;
		// std::vector<User *>		_bannedUsers;
		std::vector<User *>		_invitedUsers;
		std::vector<User *>		_operators;
};


#endif
