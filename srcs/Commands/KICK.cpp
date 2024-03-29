#include "../includes/Command.hpp"

void    Command::kick(void) {
    if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
    {
        _user.printMessage(451); //ERR_NOTREGISTEREd
        return ;
    }
    if (_input.size() < 3) {
        _user.printMessage(461, _input[0]);
        return ;
    }



    std::string channel = _input[1];
    std::string reason;
    if (_input.size() > 3) {
        for (size_t i = 3; i < _input.size(); i++)
            reason += " " + _input[i];
    }
    std::vector<std::string> listUser = split(_input[2], ',');
    for (size_t i = 0; i < listUser.size(); i++) {
        if (_user.getStatus() == User::ONLINE && _user.getChannels().size() > 0) {
            if (!_ircserv.getChannel(channel)->isOperator(_user)) {
                _user.printMessage(482, channel);
                return ;
            }
            if (!_ircserv.isChannel(channel)) {
                _user.printMessage(476, channel);
                return ;
            }
            if (!_ircserv.getChannel(channel)->isUserInChannel(listUser[i])) {
                _user.printMessage(441, listUser[i], _ircserv.getChannel(channel)->getName());
                return ;
            }
            if (_user.getNickname() == listUser[i]) {
                _user.printMessage(":" + _user.getPrefix() + " " + channel + " :Don't kick yourself, please PART if you want to leave\r\n");
                return ;
            }
            std::string msg1 = toFormat("KICK", channel + " " + listUser[i] + " :" + reason);
            std::map<int, User *> users = _ircserv.getChannel(channel)->getUsers();
            for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
			{
                if (it->second->getNickname() != listUser[i])
				{
					it->second->printMessage(msg1 + "\r\n");
                    it->second->printMessage(":" + _ircserv.getUser(listUser[i])->getPrefix() + " PART " + channel + " :KICKED\r\n");
				}
				else
				{
					_ircserv.getUser(listUser[i])->printMessage(":" + _ircserv.getUser(listUser[i])->getPrefix() + " PART " + channel + " :KICKED\r\n");
					 it->second->printMessage(msg1 + "\r\n");
				}
			}
            _ircserv.getChannel(channel)->removeUser(listUser[i]);
            _ircserv.getUser(listUser[i])->removeChannel(channel);
        }
    }
}
