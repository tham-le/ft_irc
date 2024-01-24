#include "../includes/Command.hpp"
#include "../includes/ReplyCommand.hpp"

void    Command::whois() {
    if (_user.getStatus() == User::PASSWORD_REQUIRED || _user.getStatus() == User::PASSWORD_MATCH)
    {
        _user.printMessage(451); //ERR_NOTREGISTEREd
        return ;
    }

    if (_input[0].empty()) {
        _user.printMessage(311);
        _user.printMessage(312);
        if (_user._channels.size() > 0) {
            _user.printMessage(319);
        }
        return ;
    }
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
