#include "../includes/Command.hpp"

void    Command::user() {
    if (_input.empty() || _input.size() < 5) {
        _user.printMessage(461);
        return ;
    }
    if (_user.getStatus() == User::REGISTERED || _user.getStatus() == User::ONLINE) {
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

    if (_user.getStatus() == User::PASSWORD_MATCH && _user.getNickname() != "*") {
        _user.setStatus(User::REGISTERED);
        _user.printMessage(001);
        _user.printMessage(003);
        _user.printMessage(004);
        _user.printMessage(005);
        _user.printMessage(375);
        _user.printMessage(372, "Today is a good day");
        _user.printMessage(376);
    }
    else if (_user.getStatus() != User::PASSWORD_MATCH)
        _user.printMessage("Pass required\r\n");
    else if (_user.getNickname() == "*")
        _user.printMessage("Nickname required\r\n");
}

