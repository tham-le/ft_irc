#include "../includes/Command.hpp"

void    Command::user(std::string const &msg) {
    if (msg.empty()) {
        _user.printMessage(461);
        return ;
    }
    //std::vector<std::string> str = split(msg, ' ');
    for (unsigned i = 1; i < _input.size(); i++) {
        if (strcmp(_input[i].c_str(), _ircserv._users[_user.getFd()]->getNickname().c_str()) == 0) {
            _user.printMessage(_user.getNickname() + " " + _user.getUsername() + " " + _user.getHostname());
            return ;
        }
        else {
            _user.printMessage(" ");
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
    //std::vector<std::string> str = split(msg, ' ');
    std::vector<std::string>::reverse_iterator rit = _input.rbegin();
    std::vector<std::string>::reverse_iterator rend = _input.rend() - 1;
    for (; rit != rend; ++rit) {;
        if (strcmp(rit->c_str(), _ircserv._users[_user.getFd()]->getNickname().c_str()) == 0) {
            _user.printMessage(311);
            _user.printMessage(312);
            if (_user._channels.size() > 0) {
                _user.printMessage(319);
            }
            break ;
        }
    }
}