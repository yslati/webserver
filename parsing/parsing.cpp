#include "parsing.hpp"
# include <string.h>

bool	pars::_checkbool(std::string str) {
	if (str == "on")
		return (true);
	return (false);
}

std::vector<std::string> pars::_split(std::string const &str, char sep)
{
    std::vector<std::string> wordsArr;
    std::stringstream ss(str);
    std::string buff;

    while (getline(ss, buff, sep))
        wordsArr.push_back(buff);
    return wordsArr;
}

int		pars::parsLocation(int i, int end) {

	Location	tmp;
	int			open = 0;

	tmp.setUri(_conf[i].substr(_conf[i].find(":") + 1));
	_conf[i].find("{") < _conf[i].length() ? open = 1 : open = 0;
	if ((_conf[++i] != "{" && open == 0) || (_conf[i] == "{" && open == 1)) {
		throw "Syntax Error: location `{`";
		return (i);
	}
	open = 1;
	while (open && ++i < end) {
		_conf[i] == "}" ? open = 0 : 1;
		if (_conf[i].compare(0, 8, "location") == 0 || (open == 1 && i + 1 == end))
			throw "Syntax Error: location `}`";
		if (_conf[i].compare(0, 4, "root") == 0)
			tmp.setRoot(_conf[i].substr(_conf[i].find("=") + 1));
		else if (_conf[i].compare(0, 7, "default") == 0)
			tmp.setIndex(_conf[i].substr(_conf[i].find("=") + 1));
		else if (_conf[i].compare(0, 15, "allowed_methods") == 0)
			tmp.setAllowedMethods(_split(_conf[i].substr(_conf[i].find("=") + 1), ','));
		else if (_conf[i].compare(0, 9, "autoindex") == 0)
			tmp.setAutoIndex(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		else if (_conf[i].compare(0, 9, "redirect=") == 0)
			tmp.setIsRedirect(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		else if (_conf[i].compare(0, 4, "code") == 0)
			tmp.setStatusCode(atoi(_conf[i].substr(_conf[i].find("=") + 1).c_str()));
		else if (_conf[i].compare(0, 13, "redirect_path") == 0)
			tmp.setRedirectUrl(_conf[i].substr(_conf[i].find("=") + 1));
	}
	// tmp.checkVal();
	_httpServers.addLocation(tmp);
	return (i);
}

void	pars::parsServer(int n) {

	int i = _servBegin[n];
	int port = -1;
	std::string serverName = "";

	while (++i < _servEnd[n]) {
		if (_conf[i].compare("server") == 0)
			throw "Syntax Error: You miss to Close the server `]`";
		if (_conf[i].compare(0, 4, "port") == 0 && port != -1)
			throw "Config Error: You Can't setup more than one Port";
		if (_conf[i].compare(0, 4, "port") == 0)
			port = atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str());
		else if (_conf[i].compare(0, 11, "server_name") == 0)
			serverName = _conf[i].substr(_conf[i].find(":") + 1);
		else if (_conf[i].compare(0, 4, "host") == 0)
			_httpServers.setHost(_conf[i].substr(_conf[i].find(":") + 1));
		else if (_conf[i].compare(0, 16, "allowed_methods:") == 0)
			_httpServers.setAllowedMethods(_split(_conf[i].substr(_conf[i].find(":") + 1), ','));
		else if (_conf[i].compare(0, 8, "location") == 0) {
			if (_conf[i].substr(_conf[i].find(":") + 1).compare(0, 1, "/") != 0)
				throw "Location URI error";
			i = parsLocation(i++, _servEnd[n]);
		}
		else if (_conf[i].compare(0, 10, "error_page") == 0) {
			std::string tmp =  _conf[i].substr(_conf[i].find(":") + 1);
			_httpServers.addErrorPage(atoi(tmp.c_str()), tmp.substr(tmp.find(":") + 1));
		}
	}
	_httpServers.setPort(port);
	_httpServers.setServerName(serverName);
	if (port == -1 || _httpServers.getHost() == "")
		throw "syntax err, port or Host missing!";
	else
		_Servers.addHttpServer(_httpServers);
		// _httpServers.checkVal();
}

void	pars::checkServer() {

	int serverClosed = 0;
	for (int i = 0; i < _conf.size(); i++) {
		_conf[i].erase(std::remove_if(_conf[i].begin(), _conf[i].end(), ::isspace), _conf[i].end());
		if (serverClosed == 0 && _conf[i].compare("server") == 0) {
			serverClosed = 1;
			if (_conf[i + 1].compare("[") != 0)
				break ;
			_servBegin.push_back(i + 1);
		}
		else if (serverClosed == 1 &&  _conf[i].compare("]") == 0) {
			_servEnd.push_back(i);
			serverClosed = 0;
			if ((i + 1) != _conf.size() && _conf[i + 1].compare("server") != 0)
				throw "syntax err: at end of server";
		}
	}
	for (int i = 0; i < _servBegin.size(); i++)
		parsServer(i);
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
	if (_conf.empty())
		throw "can't read the file!";
	checkServer();
}

pars::~pars() {
}
