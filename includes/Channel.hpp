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
		enum	e_ChannelMode
		{
			INVITE_ONLY,
			PUBLIC,
		};
		Channel(std::string name);
		~Channel();

		void			setName(std::string const name);
		std::string		getName() const;

		void			setTopic(std::string const topic, std::string const nickname);
		std::string		getTopic() const;

		void			setMode(e_ChannelMode mode);
		e_ChannelMode	getMode() const;

		void			setKey(std::string const key);
		std::string		getKey() const;

		void			setMaxUser(int maxUser);
		unsigned long				getMaxUser() const;


		void			addUser(User &user);
		void			removeUser(User &user);
		void			removeUser(std::string const nickname);
		User*			getUser(std::string const nickname);
		std::map<int, User *>	getUsers();
		std::string		getUsersName() const;


		bool			isUserInChannel(std::string const nickname) const;
		bool			isUserInChannel(User const &user) const;


		void			Invite(User* user);
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

		std::string			setTime();
		std::string		getCreationTime() const;
		std::string		getTopicTime() const;
		std::string		getTopicUser()const;
		std::string		getModeCmd() const;
		void			setModeCmd(std::string mode);


private:
		friend class Command;
		std::string		_name;
		std::string		_topic;
		e_ChannelMode	_mode;
		std::string		_key;
		std::string		_creationTime;
		std::string		_setTopicTime;
		std::string		_setTopicUser;
		std::string		_modeCmd;

		unsigned long			_maxUser;

		std::map<int, User *>	_users;
		// std::vector<User *>		_bannedUsers;
		std::vector<User *>		_invitedUsers;
		std::vector<User *>		_operators;
};


#endif
