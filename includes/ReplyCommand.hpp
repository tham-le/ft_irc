#ifndef REPLYCOMMAND_HPP
# define REPLYCOMMAND_HPP

/*ERROR FOR COMMAND*/
# define ERR_INPUTTOOLONG " :Input line too long\r\n" // 417
# define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n") // 431
# define ERR_ERRONEUSNICKNAME(nick) (nick + " : Erroneus nickname\r\n") // 432
# define ERR_NICKNAMEINUSE(nick) (nick + " : Nickname is already in use\r\n") // 433
# define ERR_NICKCOLLISION " : Nickname collision KILL\r\n" // 436
# define ERR_NOSUCHNICK(name) (name + " :No such nick/channel\r\n") // 401
# define ERR_UNKNOWNCOMMAND " :Unknown command\r\n" // 421
# define ERR_NOSUCHCHANNEL " :No such channel\r\n" // 403
# define ERR_NOCHANNNELS " :No channel created\r\n"
# define ERR_USERNOTINCHANNEL " :They aren't on that channel\r\n" // 441 quand on veut kick ou change mode d'un user qui n'est pas dans le channel
# define ERR_NOTONCHANNEL " :You're not on that channel\r\n" // 442
# define ERR_USERONCHANNEL " :is already on channel\r\n" // 443
# define ERR_CHANNELISFULL " :Cannot join channel (+l)\r\n" // 471
# define ERR_INVITEONLYCHAN " :Cannot join channel (You must be invited)(+i)\r\n" // 473
# define ERR_UMODEKNOWNFLAG " :Unknown MODE flag\r\n" // 501
# define ERR_INVALIDMODEPARAM " :Invalid MODE parameter\r\n" // 696
# define ERR_NEEDMOREPARAMS " :Not enough parameters\r\n" // 461


/*REPLY FOR COMMAND*/
# define RPL_WELCOME(nick) (":Welcome to the Internet Relay Network " + nick + "\r\n")
# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(date) (":This server was created " + date + "\r\n");
# define RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\r\n")

#endif