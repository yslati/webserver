#include "Client.hpp"
#include <cstdlib>
#include "Server.hpp"


Client::Client(int server_fd) {
    int len = sizeof(addr);
    _ready = false;
    _conn = accept(server_fd, (struct sockaddr*)&addr, (socklen_t*)&len);
    if (_conn < 0) {
        throw std::runtime_error("accept failed");
    }
    fcntl(_conn, F_SETFL, O_NONBLOCK);
    pfd.events = POLLIN;
    pfd.fd = _conn;
	_req = "";
//     responseContent = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nhello world";
}

bool	Client::getReady()
{
    return this->_ready;
}

bool	Client::_matchBegin(std::string _regex, std::string _line)
{
	std::string _r = _regex;
	_r.pop_back();

	return _line.compare(0, _r.size(), _r) == 0;
}

bool Client::_isPrefix(std::string& s1, std::string& s2)
{
	size_t n1 = s1.length();
	size_t n2 = s2.length();
	if (!n1 || !n2)
		return false;
	for (int i = 0; i < n1; i++)
	{
		if (s1[i] != s2[i])
		{
			if (i + 1 == n1)
				return true;
			return false;
		}
	}
	return true;
}

bool 	Client::_isSuffix(std::string s1, std::string s2)
{
	int n1 = s1.length(), n2 = s2.length();
	if (n1 > n2 || !s1.length() || !s2.length())
		return false;
	for (int i = 0; i < n1; i++)
		if (s1[n1 - i - 1] != s2[n2 - i - 1])
			return false;
	return true;
}

void	Client::_handleResponse(Request req, std::vector<HttpServer>::iterator it)
{
    std::vector<Location> Locations = it->getLocations();
	std::vector<Location>::iterator lit = Locations.begin();
	Location tmp;
	std::string u;
	std::string path = "";
	std::string uri = req._getHeaderContent("uri");


	for (; lit != Locations.end(); lit++)
	{
		if (lit->getFastcgiPass().length())
		{
			std::string _match = uri.substr(uri.find("."), uri.length());
			if (_match.find("?") != std::string::npos)
				_match = _match.substr(0, _match.find("?"));
			if ((lit->getPhpCGI() && !_match.compare(".php"))
			|| (lit->getPyCGI() && !_match.compare(".py"))
			|| (lit->getNodeCGI() && !_match.compare(".js")))
			{
				tmp = *lit;
				break ;
			}
		}
		if (_matchBegin(lit->getUri(), uri))
		{
			if (lit->getUri().compare(uri) == 0)
			{
				tmp = *lit;
				break ;
			}
			if (path.length() == 0)
			{
				path = lit->getUri();
				tmp = *lit;
			}
			else if (path.length() < lit->getUri().length())
			{
				path = lit->getUri();
				tmp = *lit;
			}
		}
	}

	Response res = Response(tmp, *it);


	res._setRequest(req);
	res._startResponse();

	responseContent = res._getResContent();
}

void	Client::_readHeader(std::string con)
{
	std::string _line;
	std::istringstream _read(con);

	while (getline(_read, _line))
	{
		if (_line.find("Host") != std::string::npos)
		{
			_req = _line.substr(_line.find("Host:") + 6);
			_req.pop_back();
		}
	}
}

void Client::_handleRequest(std::vector<HttpServer>::iterator it)
{
	Request req;

	// std::cout << content << std::endl;
	req._parseIncomingRequest(content);
	_handleResponse(req, it);
}

void	Client::setReady(bool x) {
    if (x)
	{
		// handle request
		Server& srv = Server::getInstance();
		std::vector<HttpServer> s = srv.getHttpServers();
		std::vector<HttpServer>::iterator it = s.begin();
		while (it != s.end())
		{
			_readHeader(content);
			std::string p = std::to_string(it->getPort());
			std::string h = it->getServerName();
			h.append(":").append(p);
			if (_req.compare(h) == 0)
				break ;
			it++;
		}
		_handleRequest(it);
		sended = 0;
		pfd.events = POLLOUT;
	}
	else
	{
		this->content = "";
		pfd.events = POLLIN;
	}
		this->_ready = x;
}

int Client::getConnection() {
    return this->_conn;
}

struct pollfd Client::getPfd() {
        return this->pfd;
}

int checkEnd(const std::string& str, const std::string& end)
{
        size_t  i = str.size();
        size_t  j = end.size();

        while (j > 0)
        {
                i--;
                j--;
                if (i < 0 || str[i] != end[j])
                        return (1);
        }
        return (0);
}

std::string Client::getResponseContent() {
        return this->responseContent;
}

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

void Client::writeConnection() {
	if (sended < content.size()) {
		setReady(false);
		return;
	}
	std::string toSend = responseContent.substr(sended, 128); 
	int r = send(_conn, toSend.c_str(), 128, 0); 
	if (r == 0 || r == -1) {
		throw std::runtime_error("should close the connection");
	}
	else if (r > 0) {
		sended += 128;
	}
}

Client::~Client() {
}

int Client::readConnection() {
    char buffer[1028];
    int r = recv(_conn, buffer, 128, 0);
//     return 0;
    if (r == -1) {
            return 1;
    }
    if (r == 0) {
            throw std::runtime_error("Closed");
    }
    else {
            std::string tmp;
            buffer[r] = '\0';
            tmp.assign(buffer);
            content += tmp;
            // std::cout << i << " " << content.size() << std::endl;
            // std::cout << "ENDOK" << std::endl;
            size_t j = content.find("\r\n\r\n", 0);

            // check if there is a \r\n\r\n
            if (j != std::string::npos)
            {
                    // if there isnt a content length
                    if (content.find("Content-Length: ") == std::string::npos) {
                            // check if there is chunked
                            if (content.find("Transfer-Encoding: chunked") != std::string::npos) {
                                    if (checkEnd(content, "0\r\n\r\n") == 0)
                                    {
                                            std::cout << "End" << std::endl;
                                            return (0);
                                    }
                                    else
                                            return (1);
                            }
                            else
                                    return (0);
                    }
                    size_t  len = std::atoi(content.substr(content.find("Content-Length: ") + 16, 10).c_str());
                //     size_t pos = content.find("\r\n\r\n");
                    std::cout << "Content-Length: " << len << "==" << content.substr(j + 4).size() << std::endl;
                    std::string tmp = content.substr(j + 4);
                //     tmp = ReplaceString(tmp, "\r\n", "");
                    if (tmp.size() >= len)
                            return (0);
                    else
                            return (1);
            }
    }
    return (1);
}

std::string Client::getContent() const {
    return this->content;
}

// POST /filename HTTP/1.1
// 
