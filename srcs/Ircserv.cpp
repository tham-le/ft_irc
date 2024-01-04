#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include "Ircserv.hpp"

Ircserv::Ircserv(void);
Ircserv::Ircserv(int port, std::string password);
Ircserv::Ircserv(Ircserv const &src);
Ircserv Ircserv::&operator=(Ircserv const &rhs);

Ircserv::~Ircserv();
void Ircserv::start(void);
void Ircserv::stop(void);
void Ircserv::handleMessage(std::string message);


#endif
