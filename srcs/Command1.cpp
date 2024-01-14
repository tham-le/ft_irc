#include "../includes/Command.hpp"

Command::Command(std::string const &msg, User &user, Ircserv &ircserv) : _msg(msg), _user(user), _ircserv(ircserv)
{
	initCmd();
	parse(msg);
	command();
}

Command::~Command()
{}

void		Command::initCmd()
{
	_noFunctionalOnChannel.push_back("ADMIN");
	_noFunctionalOnChannel.push_back("INFO");
	_noFunctionalOnChannel.push_back("LIST");
	_noFunctionalOnChannel.push_back("VERSION");

	_func["ADMIN"] = &Command::admin;
	// _func["INFO"] = &Command::info; //->no channel
	_func["JOIN"] =  &Command::join;
	// _func["NICK"] = &Command::nickname;
	// _func["NAMES"] = &Command::names;
	// _func["PART"] = &Command::part;
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
		// for (unsigned int i = 0; i < _input.size(); i++)
		// {
		// 	std::cout << _input[i] << "." << std::endl;
		// }
	}
}

bool		Command::isCmdNoUse(std::string const str) const
{
	return std::find(_noFunctionalOnChannel.begin(), _noFunctionalOnChannel.end(), str) != _noFunctionalOnChannel.end();
}

void		Command::command()
{
	std::map<std::string, FuncType>::iterator it = _func.find(_input[0]);

	if (it == _func.end())
	{
		if (_lastChannels.empty())
			std::cout << " Unknown command: " << _input[0] << std::endl;
		return ;
	}
	else // if (it == _func.end() && (_user.getStatus() == REGISTRED || _user.getStatus() == ONLINE))
	{
		if ((!_lastChannels.empty() && !isCmdNoUse(_input[0])) || _lastChannels.empty())
		{
			if (!_input[2].empty())
				(this->*(it->second))(_input[1]);
			// else
			// 	std::cout << "ROUCOULE" << std::endl;
		}
	}
	// else
	// {
	// 	std::cout << " "
	// }
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
/*SI le channel existe ou pas :
 Existe pas -> -cree et le met dans _channels et _lastchannels
 				-ajoute l'user dans la liste des users du channels
				-ajoute le channel dans la liste des channels du users
				-si user enrigistre du serveur et le channel a aucun user mis a part lui meme il devient operateur
				- le status de l'user devient ONLINE
 Existe -> ajoute dans last_channel
		-> verifie si user est dans la liste du channel sinon ajouter
*/
void		Command::join(std::string const &channel)
{
	int idx = 0;
	if (channel[0] == '#')
		idx = 1;

	if (_channels.find(&channel[idx]) == _channels.end())
	{
		_channels[&channel[idx]] = new Channel(channel);
		_channels[&channel[idx]]->addUser(_user);
		_channels[&channel[idx]]->addOperator(_user);
	}
	else
	{
		std::vector<Channel*>::iterator it = std::find(_lastChannels.begin(), _lastChannels.end(), _channels[channel]);

		if (it != _lastChannels.end())
			_lastChannels.erase(it);

		if (!_channels[&channel[idx]]->isUserInChannel(_user))
			_channels[&channel[idx]]->addUser(_user);
	}
	if (_channels[&channel[idx]]->isUserInChannel(_user))
	{
		_user.addChannel(_channels[&channel[idx]]);
		_lastChannels.push_back((_channels[&channel[idx]]));
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
		_lastChannels.pop_back();
		if (_lastChannels.empty())
			_user.setStatus(User::REGISTERED);
	}
	else
	{
		int idx = 0;
		if (channel[0] == '#')
			idx = 1;

		if (_channels.find(&channel[idx]) != _channels.end())
			std::cout << channel << " You're not on that channel" << std::endl;
		else
			std::cout << channel << " : No such channel" << std::endl;
	}
}

void		kick(std::string const &channel);


