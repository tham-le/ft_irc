#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstddef>
#include <map>
#include <string>



class Config
{
public:
	Config();
	~Config();

	void					setPort(int port);
	void					setPassword(std::string const password);

	int						getPort() const;
	std::string				getPassword() const;

	// void					setConfig(std::string const key, std::string const value);
	// std::string				getConfig(std::string const key) const;

	void					setPingInterval(int const pingInterval);
	int						getPingInterval() const;

	void					setPingTimeout(int const pingTimeout);
	int						getPingTimeout() const;

	void					setMaxClients(int const maxClients);
	size_t						getMaxClients() const;

	void					setMaxChannels(int const maxChannels);
	size_t						getMaxChannels() const;

private:
	// std::map<std::string, std::string>	_configList;
	int									_port;
	std::string							_password;
	int									_pingInterval;
	int									_pingTimeout;
	size_t									_maxClients;
	size_t									_maxChannels;

};

#endif // CONFIG_HPP
