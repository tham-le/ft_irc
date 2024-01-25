
//#include "../includes/User.hpp"
#include <vector>
#include <string>



class sillyBot 
{
	public:
		sillyBot(std::string host, int port, std::string password, std::string config_file);
		~sillyBot();

		void	readConfig(std::string name);
		std::string	readMessage();
		void		sendMessage(std::string message);

		void	run();
	private:
		int 			_sockfd;
		std::string		_password;
		bool			_motdEnd;
		std::vector<std::string> _jokes;
		std::vector<std::string> _quotes;
		std::vector<std::string> _astro;
};
