#include "parsing.hpp"

// void	pars::raseValue(std::string str) {
// 	if (str.compare(0, 4, "host") == 0)
// 		_httpServers.

// }

void	pars::parsServer(int n) {

	int i = _servBegin[n] + 1;
	int port = -1;
	std::string serverName = "";
	while (i < _servEnd[n]) {
		if (_conf[i].compare("server") == 0)
			std::cout << "faild " << i << std::endl;
		if ((_conf[i].compare(0, 11, "server_name") == 0 && serverName != "") || (_conf[i].compare(0, 4, "port") == 0 && port != -1))
			std::cout << "faild, port or sev already exist " << i << std::endl;
		if (_conf[i].compare(0, 4, "port") == 0)
			port = atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str());
		else if (_conf[i].compare(0, 11, "server_name") == 0)
			serverName = _conf[i].substr(_conf[i].find(":") + 1);
		else if (_conf[i].compare(0, 4, "host") == 0)
			_httpServers.setHost(_conf[i].substr(_conf[i].find(":") + 1));
		else if (_conf[i].compare(0, 16, "allowed_methods:") == 0)
			_httpServers.setAllowedMethods(_conf[i].substr(_conf[i].find(":") + 1));
		else if (_conf[i].compare(0, 8, "location") == 0) {
			// pars location
		}
		else if (_conf[i].compare(0, 10, "error_page") == 0) {
			// pars error pages
		}
		i++;
	}
	_httpServers.setPort(port);
	_httpServers.setServerName(serverName);
	if (serverName == "" || port == -1 || _httpServers.getHost() == "" || _httpServers.getAllowedMethods() == "")
		std::cout << "syntax err, somthing missing" << std::endl;
	else
		_httpServers.checkVal();
}

void	pars::checkServer() {


	int serverClosed = 0;
	for (int i = 0; i < _conf.size(); i++) {
		_conf[i].erase(std::remove_if(_conf[i].begin(), _conf[i].end(), ::isspace), _conf[i].end());
		if ( serverClosed == 0 && _conf[i].compare("server") == 0) {
			serverClosed = 1;
			if (_conf[i + 1].compare("[") != 0)
				break ;
			_servBegin.push_back(i + 1);
		}
		if (serverClosed == 1 &&  _conf[i].compare("]") == 0) {
			_servEnd.push_back(i);
			serverClosed = 0;
			if ((i + 1) != _conf.size() && _conf[i + 1].compare("server") != 0)
				std::cout << "syntax err: at end of server config" << std::endl;
		}
	}
	serverClosed == 0 ? std::cout << "success" << std::endl : std::cout << "faild" << std::endl;

	for (int i = 0; i < _servBegin.size(); i++) {
		parsServer(i);
	}
}

pars::pars(std::string fileName) {

	std::string buff;
	std::ifstream readFile(fileName);

	while (getline(readFile, buff)) {
		std::string tmp = buff.substr(0, buff.size());
		if (tmp != "")
			_conf.push_back(tmp);
	}
	readFile.close();
	checkServer();
}

pars::~pars() {
}