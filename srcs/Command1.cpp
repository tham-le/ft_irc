#include "../includes/Command.hpp"

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _msg(msg), _user(user), _ircserv(ircserv)
{
	initCmd();
	parse(msg);
	/*a supp*/
	if (_user.getStatus() == User::PASSWORD_REQUIRED)
		_user.setStatus(User::REGISTERED);
	/**/
	command();
}

Command::~Command()
{}

void		Command::initCmd()
{
	// _noFunctionalOnChannel.push_back("ADMIN");
	// _noFunctionalOnChannel.push_back("INFO");
	// _noFunctionalOnChannel.push_back("LIST");
	// _noFunctionalOnChannel.push_back("VERSION");

	_func["ADMIN"] = &Command::admin;
	// _func["INFO"] = &Command::info; //->no channel
	_func["JOIN"] =  &Command::join;
	// _func["NICK"] = &Command::nickname;
	// _func["NAMES"] = &Command::names;
	_func["PART"] = &Command::part;
	// _func["QUIT"] = &Command::quit;
	// _func["LIST"] = &Command::list; //->no channel
	// _func["KICK"] = &Command::kick;
	// _func["INVITE"] = &Command::invite;
	// _func["TOPIC"] = &Command::topic;
	// _func["MODE"] = &Command::changeMode;
	// _func["VERSION"] = &Command::version; // -> no channel
}

void		Command::parse(std::string message)
{

	if (message[0] != '/')
	{
		_input.push_back(message);
	}
	else
	{
		if (message[1] != '/')
			split(&message[1], ' ');
		else
			split(&message[2], ' ');
	}
}



void		Command::command()
{
	std::map<std::string, FuncType>::iterator it = _func.find(_input[0]);

	if (it == _func.end())
	{
		if (_user.getStatus() == User::REGISTERED)
		{
			// std::cout << "Caca" << std::endl;
			// _ircserv.writeToClient( _user.getFd(),"PIPI");
			_ircserv.writeToClient( _user.getFd(),ERR_UNKNOWNCOMMAND(_input[0]));
		}
		return ;
	}
	else // if (it == _func.end() && (_user.getStatus() == REGISTRED || _user.getStatus() == ONLINE))
	{
		if (_user.getStatus() == User::ONLINE || _user.getStatus() == User::REGISTERED)
		{
			if (_input.size() > 1)
				(this->*(it->second))(_input[1]);
			else
				(this->*(it->second))("");
		}
	}
}

void		Command::split(std::string str, char separator)
{
	int start = 0;

	for (unsigned int i = 0; i <= str.size(); i++)
	{
		if (str[i] == separator || i == str.size())
		{
			std::string arg;
			arg.append(str, start, i - start);
			_input.push_back(arg);
			start = i + 1;
		}
	}
}

void		Command::join(std::string const &channel)
{
	int idx = 0;
	if (channel.empty())
	{
		return ;
	}
	if (channel[0] == '#')
		idx = 1;

	if (!_ircserv.isChannel(&channel[idx]))
	{
		_ircserv.addChannel(&channel[idx]);
		_ircserv.getChannel(&channel[idx])->addUser(_user);

	}
	else
	{
		if (_user.isInLastChannels(_ircserv.getChannel(channel)))
			_user.removeLastChannel();

		if (!_ircserv.getChannel(&channel[idx])->isUserInChannel(_user))
			_ircserv.getChannel(&channel[idx])->addUser(_user);
	}
	if (_ircserv.getChannel(&channel[idx])->isUserInChannel(_user))
	{
		if (_ircserv.getChannel(&channel[idx])->isEmptyOperator())
			_ircserv.getChannel(&channel[idx])->addOperator(_user);
		_user.addChannel(_ircserv.getChannel(&channel[idx]));
		_user.addLastChannel((_ircserv.getChannel(&channel[idx])));
		_user.setStatus(User::ONLINE);
	}
}
/*Dans un channel :
	oui: -user change de mode ou nn si _lasChannel !empty
 		 -_lastChannel.popback
	non: - verifie si la variable channel est dans _channels
			si oui afficher nn dans le channel sinon afficher no such a channel
*/
void		Command::part(std::string const &channel)
{
	if (_user.getStatus() == User::ONLINE)
	{
		std::cout << "PIOU" << _user.getLastChannel()->getName() << std::endl;
		if (_user.getLastChannel()->isUserInChannel(_user) && _user.getLastChannel()->isOperator(_user))
			std::cout << "CACA" << std::endl;
		if (_user.getStatus() == User::ONLINE && _user.getLastChannel()->isOperator(_user))
		{
			_user.getLastChannel()->removeOperator(_user);
			// std::cout << "PART " << _lastChannels[_lastChannels.size() - 1]->getName() << std::endl;
		}
		_user.removeLastChannel();
		_user.setStatus(User::REGISTERED);
	}
	else
	{
		if (_input.size() < 2)
			std::cout << "ircserv: Not enough parameters given" << std::endl;
		else if (_ircserv.isChannel(&channel[1]) && channel[0] == '#')
			std::cout << channel << " You're not on that channel" << std::endl;
		else
			std::cout << channel << ": No such channel" << std::endl;
	}
}




/*Online sur le channel :
	->non :
			-si pas de argument < 3 : Not enough parameters given
			-si # deuxieme arg : Irssi: Not joined to any channel
			-si argv3 est pas un user du server : a: No such nick/channel
			-si tout ok ,verif si le user lui meme est dans le channel(online) si non : #lol You're not on that channel!
	->oui :
			-verif si le user a kick est dans le channel et si nous somme op remove des list

*/
// void		Command::kick(std::string const &channel)
// {

// }


