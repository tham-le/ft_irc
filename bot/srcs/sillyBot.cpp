#include "../includes/sillyBot.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
static	bool alive = true;

static void handler(int sig)
{
	(void)sig;
	alive = false;
	throw std::runtime_error("SIGINT received");

}

std::vector<std::string>	readSomeFile(std::string filename)
{
	std::vector<std::string> jokes;
	std::ifstream file(filename.c_str());

	if (!file.is_open())
		throw std::runtime_error("Cannot open file " + filename);
	std::string line;
	while (std::getline(file, line)) {
		jokes.push_back(line);
	}
	file.close();
	return jokes;
}


void	sillyBot::readConfig(std::string name)
{
	std::ifstream file(name.c_str());
	
	if (!file.is_open())
		throw std::runtime_error("Cannot open config file");
	std::string line, varName, fileName;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::getline(iss, varName, '=');
		iss >> fileName;
		if (varName == "JOKE_FILE") {
			_jokes = readSomeFile(fileName);
		} else if (varName == "ASTRO_FILE") {
			_astro = readSomeFile(fileName);
		} else if (varName == "QUOTE_FILE") {
			_quotes = readSomeFile(fileName);
		}
	}
	file.close();
}

sillyBot::sillyBot(std::string host, int port, std::string password, std::string config_file)
{
	_password = password;
	readConfig(config_file);
	_motdEnd = false;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Cannot create socket");
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	//Convert IPv4 addresses from text to binary form
	if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0)
		throw std::runtime_error("Invalid address/ Address not supported");

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw std::runtime_error("Connection failed");
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	_sockfd = sockfd;
	std::cout << "Connected to " << host << ":" << port << std::endl;
}

std::string sillyBot::readMessage()
{
	char buffer[1024];
	int n = read(_sockfd, buffer, 1024);
	if (n < 0)
		return "";
	buffer[n] = '\0';
	return std::string(buffer);
}

void		sillyBot::sendMessage(std::string message)
{
	int n = write(_sockfd, message.c_str(), message.length());
	if (n < 0)
		throw std::runtime_error("Cannot write to socket");
}

void	sillyBot::run()
{
	if (!alive)
		return ;
	
	std::cout << "Running bot" << std::endl;
	
	// Send password


	sendMessage("NICK sillyBot \r\nPASS " + _password + "\r\nUSER sillyBot 0 * :sillyBot \r\n");


	// Main loop
	signal(SIGINT, handler);
	while (alive)
	{
		srand(time(NULL));
		std::string message = readMessage();
		
		if (message != "")
			std::cout << message << std::endl;
		if (message.find("PING") != std::string::npos)
			sendMessage("PONG " + message.substr(5) + "\r\n");
		if (message.find("376") != std::string::npos)
		{
			sendMessage("JOIN #sillyBot\r\n");
			_motdEnd = true;
		}

		if ( _motdEnd && message.find("PRIVMSG") != std::string::npos)
		{
			std::string sender = message.substr(1, message.find("!") - 1);
			std::string channel = message.substr(message.find("PRIVMSG") + 8, message.find(":", message.find("PRIVMSG")) - message.find("PRIVMSG") - 9);
			std::string msg = message.substr(message.find(":", message.find("PRIVMSG")) + 1);
			if (msg.find("!joke") != std::string::npos)
			{
				int index = rand() % _jokes.size();
				sendMessage("PRIVMSG " + channel + " :" + _jokes[index] + "\r\n");
			}
			else if (msg.find("!quote") != std::string::npos)
			{
				int index = rand() % _quotes.size();
				sendMessage("PRIVMSG " + channel + " :" + _quotes[index] + "\r\n");
			}
			else if (msg.find("!astro") != std::string::npos)
			{
				int index = rand() % _astro.size();
				sendMessage("PRIVMSG " + channel + " :" + _astro[index] + "\r\n");
			}
			else if (msg.find("!help") != std::string::npos)
			{
				sendMessage("PRIVMSG " + channel + " :Commands: !joke, !quote, !astro, !help\r\n");
			}
		}
	}
}



sillyBot::~sillyBot()
{
}
