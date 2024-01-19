#ifndef REPLYCOMMAND_HPP
# define REPLYCOMMAND_HPP

/*ERROR FOR COMMAND*/
# define ERR_INPUTTOOLONG() (" :Input line too long\r\n") // 417
# define ERR_NONICKNAMEGIVEN() (" :No nickname given\r\n") // 431
# define ERR_ERRONEUSNICKNAME(nick) (nick + " : Erroneus nickname\r\n") // 432
# define ERR_NICKNAMEINUSE(nick) (nick + " : Nickname is already in use\r\n") // 433
# define ERR_NICKCOLLISION(nick) (nick + " : Nickname collision KILL\r\n") // 436
# define ERR_NOSUCHNICK(name) (name + " :No such nick/channel\r\n") // 401
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n") // 421
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n") // 403
# define ERR_NOCHANNNELS() (" :No channel created\r\n") //notre propre message d'erreur, n'existe pas dans le doc d'IRC
# define ERR_USERNOTINCHANNEL(nick, channel) (nick + " " + channel + " :They aren't on that channel\r\n") // 441 quand on veut kick ou change mode d'un user qui n'est pas dans le channel
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n") // 442
# define ERR_USERONCHANNEL(nick, channel) (nick + " " + channel + " :is already on channel\r\n") // 443
# define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)\r\n") // 471
# define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (You must be invited)(+i)\r\n") // 473
# define ERR_UMODEKNOWNFLAG() (" :Unknown MODE flag\r\n") // 501
# define ERR_INVALIDMODEPARAM() (" :Invalid MODE parameter\r\n") // 696
# define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters\r\n") // 461
# define ERR_ALREADYREGISTRED() (" :You may not reregister\r\n") // 462
# define ERR_NOTJOINEDANYCHANNEL() (" :Not joined to any channel\r\n") //notre propre message d'erreur, n'existe pas dans le doc d'IRC
# define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)")// 475



/*REPLY FOR COMMAND*/
# define RPL_WELCOME(nick) ("Welcome to the Internet Relay Network " + nick + "\r\n") //001
# define RPL_YOURHOST(servername, version) ("Your host is " + servername + ", running version " + version + "\r\n") //002
# define RPL_CREATED(date) ("This server was created " + date + "\r\n"); //003
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n") //004
# define RPL_ISUPPORT() (":005\r\n") //005

# define RPL_BOUNCE(server_name, port) ("010 " + "Try server " + server_name + ", port " + port + "\r\n") //010
# define RPL_WHOISUSER(nick, user, host, real_name) (nick + " " + user + " " + host + " * :" + real_name + "\r\n") //311
# define RPL_WHOISSERVER(nick, servername, version) (nick + " " + servername + " :" + version + "\r\n") //312
# define RPL_WHOISCHANNELS(nick, channels) (nick + " :" + channels + "\r\n") //319
# define RPL_ENDOFWHOIS() (" :End of /WHOIS list\r\n") //318
# define RPL_LISTSTART() ("Channel :Users Name\r\n") //321
# define RPL_LIST(channel, nb_users, topic) (channel + " " + nb_users + " :" + topic + "\r\n") //322
# define RPL_LISTEND() ("End of /LIST\r\n") //323
# define RPL_CHANNELMODEIS(channel, mode) (channel + " " + mode + "\r\n") //324
# define RPL_CREATIONTIME(channel, date) (channel + " " + date + "\r\n") //329
# define RPL_NOTOPIC(channel) (channel +  ":No topic is set\r\n") // 331
# define RPL_TOPIC(channel, subject) ("Topic for #" + channel + ": " + subject + "\r\n") // 332
# define RPL_TOPICWHOTIME(user, time) ("Topic set by " + user + " [" + time + "]" + "\r\n")// 333
# define RPL_NAMREPLY(channel, nick) (channel + " :" + nick + "\r\n") //353
# define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list\r\n") //366

#endif
