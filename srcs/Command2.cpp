#include "../includes/Command.hpp"

void    Command::user(std::string const &msg) {
    if (msg.empty()) {
        _user.printMessage(461);
        return ;
    }
    std::vector<std::string> str = split(msg, ' ');
    for (unsigned i = 0; i < str.size(); i++) {
        if (strcmp(str[i].c_str(), _ircserv._users[_user.getFd()]->getNickname().c_str()) == 0) {
            _ircserv.writeToClient(_user.getFd(), _user.getNickname() + " " + _user.getUsername() + " " + _user.getHostname() + " *");
            return ;
        }
        else {
            _ircserv.writeToClient(_user.getFd(), " ");
        }
    }
}

void    Command::whois(std::string const &msg) {
    if (msg.empty()) {
        _user.printMessage(311);
        _user.printMessage(312);
        if (_user._channels.size() > 0) {
            _user.printMessage(319);
        }
        return ;
    }
    std::vector<std::string> str = split(msg, ' ');
    if (strcmp(str.back().c_str(), _ircserv._users[_user.getFd()]->getNickname().c_str()) == 0) {
        _user.printMessage(311);
        _user.printMessage(312);
        if (_user._channels.size() > 0) {
            _user.printMessage(319);
        }
        return ;
    }
    else {
        _ircserv.writeToClient(_user.getFd(), " ");
    }
}