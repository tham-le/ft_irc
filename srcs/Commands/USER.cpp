#include "../includes/Command.hpp"

void    Command::user() {
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
}

