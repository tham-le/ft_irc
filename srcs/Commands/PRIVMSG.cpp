#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void		Command::privmsg(void) {
    if (_input.size() > 7)
        _user.printMessage(407); // ERR_TOOMANYTARGETS (5 targets max, 7th is the message)
    for (unsigned int i = 1; i < _input.size() - 1; i++) {
        std::string target = _input[i];
        if (target[0] == '#')
            target = target.substr(1);
        else if (target[0] == '@' && target[1] == '%' && target[2] == '#') // @%#channel to send msg to opers on channel
            target = target.substr(3);
        if (!_ircserv.isUser(target) || !_ircserv.isChannel(target)) {
                _user.printMessage(401, target); // ERR_NOSUCHNICK
                return ;
            }
            else {
                _user.printMessage(411, _input[0]); // ERR_NORECIPIENT
                return ;
            }
        if (_input[_input.size() - 1][0] != ':') {
            _user.printMessage(412); // ERR_NOTEXTTOSEND
            return ;
        }
        else if (_ircserv.isUser(target)) {
            if (_user.getStatus() == User::ONLINE) {
                User *user = _ircserv.getUser(target);
                user->printMessage(toFormat("PRIVMSG", _input[_input.size() - 1]));
            }
            else
                _user.printMessage(301, target, _input[_input.size() - 1]); // should code for send RPL_AWAY if user is away
        }
        else if (_ircserv.isChannel(target)) {
            Channel *channel = _ircserv.getChannel(target);
            std::map<int, User *> listUsers = channel->getUsers();
            if (_input[i][0] == '@' && _input[i][1] == '%' && _input[i][2] == '#') {
                for (std::map<int, User *> ::iterator it = listUsers.begin(); it != listUsers.end(); it++)
                    if (it->second->isOperator())
                        it->second->printMessage(toFormat("PRIVMSG", _input[_input.size() - 1])); //send msg to opers on channel
            }
            else {
                for (std::map<int, User *> ::iterator it = listUsers.begin(); it != listUsers.end(); it++)
                    it->second->printMessage(toFormat("PRIVMSG", _input[_input.size() - 1]));  //how print to channel? send for all clients on channel?
            }
        }
    }
}