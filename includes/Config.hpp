#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <string>



class Config
{
public:
	Config();
	~Config();

	void					setPort(std::string const port);
	void					setPassword(std::string const password);

	int						getPort() const;
	std::string				getPassword() const;

	void					setConfig(std::string const key, std::string const value);
	std::string				getConfig(std::string const key) const;

	void					setPingInterval(int const pingInterval);
	int						getPingInterval() const;

	void					setPingTimeout(int const pingTimeout);
	int						getPingTimeout() const;

	void					setMaxClients(int const maxClients);
	int						getMaxClients() const;

private:
	std::map<std::string, std::string>	_configList;
	int									_port;
	std::string							_password;
	int									_pingInterval;
	int									_pingTimeout;
	int									_maxClients;

};

#endif // CONFIG_HPP
