#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void		Command::privmsg(void) {
    std::string msg;
    bool colon = false;
    for (unsigned long i = 0; i != _input.size(); i++) {
        if (_input[i][0] == ':') {
            colon = true;
            msg += _input[i].substr(1);
        }
        else if (colon)
            msg += " " + _input[i];
    }
    for (unsigned int i = 1; i < _input.size(); i++) {
        std::string target = _input[i];
        if (!_ircserv.isUser(_input[1]) && !_ircserv.isChannel(_input[1])) {
                _user.printMessage(401, target); // ERR_NOSUCHNICK
                return ;
        }
        else if (_ircserv.isUser(target)) {
            if (_user.getStatus() == User::ONLINE) {
                User *user = _ircserv.getUser(target);
                user->printMessage(toFormat("PRIVMSG", user->getNickname(), msg));
            }
            else
                _user.printMessage(301, target, msg); // should code for send RPL_AWAY if user is away
        }
        else if (_ircserv.isChannel(target)) {
            Channel *channel = _ircserv.getChannel(target);
            std::map<int, User *> listUsers = channel->getUsers();
            for (std::map<int, User *> ::iterator it = listUsers.begin(); it != listUsers.end(); it++) {
                if (_user.getNickname() == it->second->getNickname())
                    continue ;
                it->second->printMessage(toFormat("PRIVMSG", target, msg));  //how print to channel? send for all clients on channel?
            }
        }
    }
}
