#include "../includes/Command.hpp"
#include <iostream>

void    Command::user() {
    std::cout << "user" << std::endl;
    if (_input.empty() || _input.size() < 5) {
        _user.printMessage(461);
        return ;
    }
    if (_user.getStatus() == User::ONLINE) {
        _user.printMessage(462);
        return ;
    }
    _user.setUsername(_input[1]);
    _user.setMode(atoi(_input[2].c_str()));

    std::string realname;
    for (size_t i = 4; i < _input.size(); i++) {
    	realname += _input[i];
    }
    _user.setRealname(realname);
		
    std::cout << "User " << _user.getUsername() << " is now registered" << std::endl;
}

void    Command::whois() {
    if (_input[0].empty()) {
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
