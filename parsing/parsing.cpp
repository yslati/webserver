#include "parsing.hpp"

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

int		pars::parsLocation(int i, int end, HttpServer& srv) {

	Location	tmp;
	int			open = 0;

	tmp.setUri(_conf[i].substr(_conf[i].find(":") + 1));
	_conf[i].find("{") < _conf[i].length() ? open = 1 : open = 0;
	if ((_conf[++i] != "{" && open == 0) || (_conf[i] == "{" && open == 1))
		throw "Syntax Error: location `{`";
	open = 1;
	while (open && ++i < end) {
		_conf[i] == "}" ? open = 0 : 1;
		if (_conf[i].compare(0, 8, "location") == 0 || (open == 1 && i + 1 == end))
			throw "Syntax Error: location `}`";
		if (_conf[i].compare(0, 4, "root") == 0) {
			if (tmp.getRoot() != "")
				throw "Syntax Error: location: 'root' duplicated";
			tmp.setRoot(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 5, "index") == 0) {
			if (tmp.getIndex() != "")
				throw "Syntax Error: location: 'index' duplicated";
			tmp.setIndex(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 15, "allowed_methods") == 0) {
			if (!tmp.getAllowedMethod().empty())
				throw "Syntax Error: location: 'allowed_methods' duplicated";
			tmp.setAllowedMethods(_split(_conf[i].substr(_conf[i].find("=") + 1), ','));
		}
		else if (_conf[i].compare(0, 9, "autoindex") == 0) {
			if (tmp.getAutoIndex() != false)
				throw "Syntax Error: location: 'autoindex' duplicated";
			tmp.setAutoIndex(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		}
		else if (_conf[i].compare(0, 9, "redirect=") == 0) {
			if (tmp.getIsRedirect() != false)
				throw "Syntax Error: location: 'redirect' duplicated";
			tmp.setIsRedirect(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		}
		else if (_conf[i].compare(0, 4, "code") == 0) {
			if (tmp.getStatusCode() != -1)
				throw "Syntax Error: location: 'status Code' duplicated";
			tmp.setStatusCode(atoi(_conf[i].substr(_conf[i].find("=") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 13, "redirect_path") == 0) {
			if (tmp.getRedirectUrl() != "")
				throw "Syntax Error: location: 'redirect_path' duplicated";
			tmp.setRedirectUrl(_conf[i].substr(_conf[i].find("=") + 1));
		}
	}
	if (tmp.getIsRedirect() == true && (tmp.getStatusCode() == -1 || tmp.getRedirectUrl() == ""))
		throw "you need to setup redirect code and index";
	srv.addLocation(tmp);
	return (i);
}

void	pars::_check_missing(HttpServer &srv) {
	if (srv.getPort() == -1)
		throw "syntax err: Port Not found!";
	else if (srv.getHost() == "")
		throw "syntax err: Host Not found!";
	else if (srv.getRoot() == "")
		throw "syntax err: Root Not found!";
}

bool pars::_isNumber(const std::string& str)
{
    for (int i = 0; i < str.length(); i++) {
        if (std::isdigit(str[i]) == 0) return false;
    }
    return true;
}

void	pars::parsServer(int n) {

	int i = 		_servBegin[n];
	HttpServer		_httpServers;

	while (++i < _servEnd[n]) {
		if (_conf[i].compare("server") == 0)
			throw "Syntax Error: You miss to Close the server `]`";
		if (_conf[i].compare(0, 4, "port") == 0) {
			if (_httpServers.getPort() != -1)
				throw "Syntax Error: 'Port' duplicated";
			if (!_isNumber(_conf[i].substr(_conf[i].find(":") + 1)))
				throw "Port must be number ";
			_httpServers.setPort(atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 11, "server_name") == 0) {
			if (_httpServers.getServerName() != "")
				throw "Syntax Error: 'server_name' duplicated";
			_httpServers.setServerName(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 4, "host") == 0) {
			if (_httpServers.getHost() != "")
				throw "Syntax Error: 'host' duplicated";
			_httpServers.setHost(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 4, "root") == 0) {
			if (_httpServers.getRoot() != "")
				throw "Syntax Error: 'root' duplicated";
			_httpServers.setRoot(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 20, "client_max_body_size") == 0) {
			if (_httpServers.getMaxBodySize() != -1)
				throw "Syntax Error: 'client_max_body_size' duplicated";
			_httpServers.setMaxBodySize(atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 16, "allowed_methods:") == 0) {
			if (!_httpServers.getAllowedMethods().empty())
				throw "Syntax Error: 'alowed_method' duplicated";
			_httpServers.setAllowedMethods(_split(_conf[i].substr(_conf[i].find(":") + 1), ','));
		}
		else if (_conf[i].compare(0, 8, "location") == 0) {
			if (_conf[i].substr(_conf[i].find(":") + 1).compare(0, 1, "/") != 0)
				throw "Location URI error";
			i = parsLocation(i++, _servEnd[n], _httpServers);
		}
		else if (_conf[i].compare(0, 10, "error_page") == 0) {
			std::string tmp =  _conf[i].substr(_conf[i].find(":") + 1);
			_httpServers.addErrorPage(atoi(tmp.c_str()), tmp.substr(tmp.find(":") + 1));
		}
		else if (_conf[i].find("#"))
			throw "Syntax Error !";
	}
	_check_missing(_httpServers);
	_Servers.addHttpServer(_httpServers);
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

pars::pars(std::string fileName): _Servers(Server::getInstance()) {

	std::string buff;
	std::ifstream readFile(fileName);

	if (access( fileName.c_str(), F_OK ))
		throw "file doesn't exist!";
	if (fileName.compare(fileName.length() - 5, fileName.length(), ".conf") != 0)
		throw "config file must be '.conf'";
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
