#ifndef REPLYCOMMAND_HPP
# define REPLYCOMMAND_HPP

/*ERROR FOR COMMAND*/
# define ERR_INPUTTOOLONG() (" :Input line too long") // 417
# define ERR_NONICKNAMEGIVEN() (" :No nickname given") // 431
# define ERR_ERRONEUSNICKNAME(nick) (nick + " :Erroneus nickname") // 432
# define ERR_NICKNAMEINUSE(nick) (nick + " : Nickname is already in use") // 433
# define ERR_NICKCOLLISION(nick) (nick + " : Nickname collision KILL") // 436
# define ERR_NOSUCHNICK(name) (name + " :No such nick/channel") // 401
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command") // 421
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel") // 403
# define ERR_NOCHANNNELS() (" :No channel created") //notre propre message d'erreur, n'existe pas dans le doc d'IRC
# define ERR_USERNOTINCHANNEL(nick, channel) (nick + " " + channel + " :They aren't on that channel") // 441 quand on veut kick ou change mode d'un user qui n'est pas dans le channel
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel") // 442
# define ERR_USERONCHANNEL(nick, channel) (nick + " " + channel + " :is already on channel") // 443
# define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)") // 471
# define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (You must be invited)(+i)") // 473
# define ERR_UMODEKNOWNFLAG() (" :Unknown MODE flag") // 501
# define ERR_INVALIDMODEPARAM() (" :Invalid MODE parameter") // 696
# define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters") // 461
# define ERR_ALREADYREGISTRED() (" :You may not reregister") // 462
# define ERR_NOTJOINEDANYCHANNEL() (" :Not joined to any channel") //notre propre message d'erreur, n'existe pas dans le doc d'IRC
# define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)")// 475
# define ERR_NOTREGISTERED() (":You have not registered"); // 451
# define ERR_TOOMANYCHANNELS(channel) (":You have joined too many channels") //405
# define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator") //482



/*REPLY FOR COMMAND*/
# define RPL_WELCOME(nick) ("Welcome to the Internet Relay Network " + nick ) //001
# define RPL_YOURHOST(servername, version) ("Your host is " + servername + ", running version " + version ) //002
# define RPL_CREATED(date) ("This server was created " + date ); //003
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes ) //004
# define RPL_ISUPPORT() (": You are registered now, start your adventure") //005

# define RPL_BOUNCE(server_name, port) ("010 " + "Try server " + server_name + ", port " + port ) //010
# define RPL_WHOISUSER(nick, user, host, real_name) (nick + " " + user + " " + host + " * :" + real_name ) //311
# define RPL_WHOISSERVER(nick, servername, version) (nick + " " + servername + " :" + version ) //312
# define RPL_WHOISCHANNELS(nick, channels) (nick + " :" + channels ) //319
# define RPL_ENDOFWHOIS() (" :End of /WHOIS list") //318
# define RPL_LISTSTART() ("Channel :Users Name") //321
# define RPL_LIST(channel, nb_users, topic) (channel + " " + nb_users + " :" + topic ) //322
# define RPL_LISTEND() ("End of /LIST") //323
# define RPL_CHANNELMODEIS(channel, mode) (channel + " " + mode ) //324
# define RPL_CREATIONTIME(channel, date) (channel + " " + date ) //329
# define RPL_NOTOPIC(channel) (channel +  ":No topic is set") // 331
# define RPL_TOPIC(channel, topic) (channel + " :" + topic ) // 332
# define RPL_TOPICWHOTIME(user, time) ("Topic set by " + user + " [" + time + "]" )// 333
# define RPL_NAMREPLY(channel, nick) (channel + " :" + nick ) //353
# define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list") //366


// enum ReplyCode
// {
// 	/*ERROR FOR COMMAND*/
// 	ERR_INPUTTOOLONG = 417,
// 	ERR_NONICKNAMEGIVEN = 431,
// 	ERR_ERRONEUSNICKNAME = 432,
// 	ERR_NICKNAMEINUSE = 433,
// 	ERR_NICKCOLLISION = 436,
// 	ERR_NOSUCHNICK = 401,
// 	ERR_UNKNOWNCOMMAND = 421,
// 	ERR_NOSUCHCHANNEL = 403,
// 	ERR_NOCHANNNELS = 404,
// 	ERR_USERNOTINCHANNEL = 441,
// 	ERR_NOTONCHANNEL = 442,
// 	ERR_USERONCHANNEL = 443,
// 	ERR_CHANNELISFULL = 471,
// 	ERR_INVITEONLYCHAN = 473,
// 	ERR_UMODEKNOWNFLAG = 501,
// 	ERR_INVALIDMODEPARAM = 696,
// 	ERR_NEEDMOREPARAMS = 461,
// 	ERR_ALREADYREGISTRED = 462,
// 	ERR_NOTJOINEDANYCHANNEL = 444,
// 	ERR_BADCHANNELKEY = 475,
// 	ERR_NOTREGISTERED = 451,
// 	ERR_TOOMANYCHANNELS = 405,
// 	ERR_CHANOPRIVSNEEDED = 482,

// 	/*REPLY FOR COMMAND*/
// 	RPL_WELCOME = 1,
// 	RPL_YOURHOST = 2,
// 	RPL_CREATED = 3,
// 	RPL_MYINFO = 4,
// 	RPL_ISUPPORT = 5,
// 	RPL_BOUNCE = 10,
// 	RPL_WHOISUSER = 311,
// 	RPL_WHOISSERVER = 312,
// 	RPL_WHOISCHANNELS = 319,
// 	RPL_ENDOFWHOIS = 318,
// 	RPL_LISTSTART = 321,
// 	RPL_LIST = 322,
// 	RPL_LISTEND = 323,
// 	RPL_CHANNELMODEIS = 324,
// 	RPL_CREATIONTIME = 329,
// 	RPL_NOTOPIC = 331,
// 	RPL_TOPIC = 332,
// 	RPL_TOPICWHOTIME = 333,
// 	RPL_NAMREPLY = 353,
// 	RPL_ENDOFNAMES = 366
// };

#endif
