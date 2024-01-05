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

private:
	std::map<std::string, std::string>	_configList;
	int									_port;
	std::string							_password;

};

#endif // CONFIG_HPP
