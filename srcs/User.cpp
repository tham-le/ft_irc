#include "../includes/User.hpp"

User::User() {
    _nickname = "";
    _username = "";
    _realname = "";
    _hostname = "";
    _isOperator = false;
    _isAway = false;
    _awayMessage = "";
    _buffer = "";
}

User::User(int fd, struct sockaddr_in addr) {
    _fd = fd;
    _hostname = inet_ntoa(addr.sin_addr);
    _isOperator = false;
    _isAway = false;
    _awayMessage = "";
}

User::~User() {}

void User::setNickname(std::string nickname) {
    try {
        if (nickname.length() > 9)
            throw std::runtime_error("Nickname too long");
        else if (nickname.length() < 1)
            throw std::runtime_error("Nickname too short");
        for (std::string::iterator it = nickname.begin(); it != nickname.end(); it++)
            if (std::isspace(*it))
                throw std::runtime_error("Nickname cannot contain spaces");
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ;
    }
    _nickname = nickname;
}

void User::setUsername(std::string username) {
    _username = username;
}

void User::setRealname(std::string realname) {
    _realname = realname;
}

void User::setHostname(std::string hostname) {
    _hostname = hostname;
}

std::string User::getNickname() const {
    return _nickname;
}

std::string User::getUsername() const {
    return _username;
}

std::string User::getRealname() const {
    return _realname;
}

std::string User::getHostname() const {
    return _hostname;
}

int User::getFd() const {
    return _fd;
}

bool User::isOperator() const {
    return _isOperator;
}

void User::setOperator(bool isOperator) {
    _isOperator = isOperator;
}

bool User::isAway() const {
    return _isAway;
}

void User::setAway(bool isAway) {
    _isAway = isAway;
}

void User::setAwayMessage(std::string awayMessage) {
    _awayMessage = awayMessage;
}

std::string User::getAwayMessage() const {
    return _awayMessage;
}


void User::sendMessage(User *user, std::string message) {
    std::cout << "Sending message to " << user->getNickname() << ": " << message << std::endl;
    // send(user->getFd(), message.c_str(), message.length(), 0);
}
