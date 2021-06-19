#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex>
// #include <bits/stdc++.h>
#include <algorithm>
#include <cstdlib>

struct Arg {
    std::string _Cdisp;
    std::string _Ctype;
    std::string _data;
};

std::string _regex;
std::smatch match;
std::vector<Arg> _vec;
bool _isArg;
std::string _body = "";

Arg parseArg(std::string& content)
{
    Arg arg = {};
    std::string _line;
    std::istringstream _read(content);
    
    while (getline(_read, _line))
    {
        if (!_line.length())
            continue ;
        if (_line.find("Content-Disposition") != std::string::npos)
			arg._Cdisp = _line.substr(_line.find(":") + 2);
		else if (_line.find("Content-Type") != std::string::npos)
			arg._Ctype = _line.substr(_line.find(":") + 2);
		else if (_line.length() && _line[0] != '\r')
		{
			 if (arg._data.length())
				arg._data.append("\n");
			arg._data = arg._data.append(_line);
		}
    }
    return (arg);
}

bool _IsSuffix(std::string s1, std::string s2)
{
	int n1 = s1.length(), n2 = s2.length();
	if (n1 > n2 || !s1.length() || !s2.length())
		return false;
	for (int i = 0; i < n1; i++)
		if (s1[n1 - i - 1] != s2[n2 - i - 1])
			return false;
	return true;
}


bool _IsPreffix(std::string s1, std::string s2)
{
	int n1 = s1.length(), n2 = s2.length();
	if (!s1.length() || !s2.length())
		return false;
	for (int i = 0; i < n1; i++)
		if (s1[i] != s2[i])
		{
			if (i + 1 == n1)
				return true;
			return false;
		}
	return true;
}

void _appendToVec(std::string& content)
{
    content.pop_back();
    // std::regex re(_regex);
    // std::regex_search(content, match, re);
    // std::cout << "con = " << content << "\n";
    if (_regex.length() && _IsSuffix(_regex, content))
    {
        // std::cout << "suffix = " << content << "\n";
        if (!_isArg)
            _isArg = true;
        else
        {
            _vec.push_back(parseArg(_body));
            _body.clear();
        }
    }
    else if (_IsPreffix(_regex, content))
    {
        std::cout << "prefix = " << _body << "\n";
        _isArg = false;
        _vec.push_back(parseArg(_body));
        _body.clear();
    }
    else if (_isArg)
    {
        _body.append(content).append("\n");
        // std::cout << "_body = " << _body << "\n";
    }
    else
        _isArg = true;
}

bool _matchBegin(std::string _regex, std::string _line)
{
    std::string _r = _regex;
    _r.pop_back();;
    return _line.compare(0, _r.length(), _r) == 0;
}

bool _matchEnd(std::string s1, std::string s2)
{
    int n1 = s1.length(), n2 = s2.length();
	if (n1 > n2 || !s1.length() || !s2.length())
		return false;
	for (int i = 0; i < n1; i++)
		if (s1[n1 - i - 1] != s2[n2 - i - 1])
			return false;
	return true;
}

void _addToVec(std::string content)
{
    /*if (_regex.length() && _IsSuffix(_regex, content))
    {
    }*/
    // std::cout << "pre = " << _IsPreffix(_regex, content) << "\n";
    // std::cout << "match = " << _matchBegin(_regex, content) << "\n";
    // std::cout << "c = " << content << std::endl;
    // std::cout << "r = " << _regex << std::endl;
    content.pop_back();
    if (_regex.length() && _matchBegin(_regex, content))
    {
        // _isArg = true;
        _vec.push_back(parseArg(_body));
        _body.clear();
    }
    else if (_isArg)
        _body.append(content).append("\n");
    else
        _isArg = true;
}

std::string _getFile(std::string disp)
{
    std::string path;
    if (disp.length())
        path = disp.substr(disp.find("filename=\"") + 10, disp.length() - disp.substr(0, disp.find("filename") + 11).length());
    return (path);
}

std::string _getContentType(std::string uri, std::string u)
{
    if (_matchEnd(".html", uri))
        return "text/html";
    else if (_matchEnd(".css", uri))
        return "text/css";
    else if (_matchEnd(".js", uri))
        return "text/js";
    else if (_matchEnd(".json", uri))
        return "application/json";
    else if (_matchEnd(".xml", uri))
        return "application/xml";
    else if (u.compare("some") == 0)
        return u;
    else
        return "text/plain";
}

// int main()
// {
    // std::string t = "Content-Type: text/html; charset=UTF-8";
    // std::string b = "Content-Type: multipart/form-data; boundary=---------------------------974767299852498929531610575";
    // std::string k = "";

    // std::string g = t.substr(t.find_first_of(':') + 2, t.find_first_of(';') - 14);
    // std::cout << "f0 = " << t.find_first_of(':') + 2 << std::endl;
    // std::cout << "f = " << t.find_first_of(';') << std::endl;
    // std::cout << b.substr(b.find("boundary=") + 9) << std::endl;
    // k = k.append("--").append(b.substr(b.find("boundary=") + 9));
    // std::cout << k << std::endl;
    // std::cout << b << std::endl;

    /*char buff[1024];
    getcwd(buff, sizeof(buff));

    std::string dir = buff;

    dir.append("/includes/_dhtml/index.html");
    std::fstream readFile(dir);
    std::string _line;
    std::string body = "";

    while (getline(readFile, _line))
        body.append(_line).append("\n");
    
    std::cout << body;*/
    // std::vector<struct Arg> v;

    // struct Arg f;
    // struct Arg f1;

    // f.Cdisp = "Content-Decposition: text/plain; charset=ascii";
    // f.Ctype = "Content-Type: text/html";
    // f.Ftype = "form-data";

    // f1.Cdisp = "Content-Decposition: text/html; charset=utf-8";
    // f1.Ctype = "Content-Type: image/png";
    // f1.Ftype = "json";
    // v.push_back(f);
    // v.push_back(f1);

    // std::vector<struct Arg>::iterator it = v.begin();

    // for (; it != v.end(); it++)
    // {
    //     std::cout << "Cdisp = " << it->Cdisp << "\n";
    // }

    /*std::string t = "\r\n";
    t += "----------------------------eb32ead89fa23a33\r\n";
    _regex = "----------------------------eb32ead89fa23a33\r\n";
    _regex.pop_back();
    t += "Content-Disposition: form-data; name=\"fname\"\r\n";
    t += "\r\n";
    t += "Content-Type: text/plain\r\n";
    t += "ayoub\r\n";
    // t += "--------------------------eb32ead89fa23a33--\r\n";
    t += "----------------------------eb32ead89fa23a33\r\n";
    t += "Content-Disposition: form-data; filename=\"fname.txt\"\r\n";
    t += "\r\n";
    t += "Content-Type: text/ob-stream\r\n";
    t += "lodush=hayar\r\n";
    t += "----------------------------eb32ead89fa23a33--\r\n";
    _isArg = false;

    std::string _line;
    std::istringstream _read(t);

    while (getline(_read, _line))
    {
        // _appendToVec(_line);
        _addToVec(_line);
        std::regex re(s);
        std::smatch match;

        std::regex_search(t, match, re);

        if (match.suffix().length())
        {
            std::cout << "suffix = " << match.suffix() << "\n";
        }
        std::regex r(_line);
        std::smatch m;

        std::regex_search(t, m, r);
        if (m.prefix().length())
            std::cout << "preffix = " << m.prefix() << "\n";
        // std::cout << "_line = " << _line << "\n";
    }
    std::cout << "s = " << _vec.size() << std::endl;
    std::regex re("filename=\"");
    std::smatch match;
    for (int i = 0; i < _vec.size(); i++)
    {
        Arg arg = _vec[i];
        std::regex_search(arg._Cdisp, match, re);
        if (!match.empty())
         {
             std::string _file = match.suffix();
             _file = _file.substr(0, _file.find("\""));
             std::cout << _file << std::endl;
         }
        // std::string _file = _getFile(arg._Cdisp);
        // std::cout << "filename = " << _file << std::endl;
    }

    std::string _Cdisp = "form-data; name=\"myfile\"; filename=\"commanf.txt\"";*/
    // std::string p = g.substr(0, g.find("filename") + 11);
    // std::string type = _Cdisp.substr(_Cdisp.find("filename=\"") + 10,
	// _Cdisp.length() - _Cdisp.substr(0, _Cdisp.find("filename") + 11).length());
    // std::cout << "size = " << g.length() - p.length() << std::endl;
    // std::cout << "size = " << p << std::endl;
    // std::cout << type << std::endl;
    /*DIR *dir;
    struct dirent *pDirent;

    if (!(dir = opendir(_getCurrentDir().c_str())))
        std::cout << "hey\n";
    while ((pDirent = readdir(dir)) != NULL)
    {
		if (pDirent->d_type == DT_DIR)
			std::cout << pDirent->d_name << "/" << std::endl;
		else
        	std::cout << pDirent->d_name << std::endl;
    }

    closedir(dir);*/
//}


std::string _getCurrentDir()
{
	char buff[1024];

	getcwd(buff, sizeof(buff));

	return buff;
}

std::string _generateHtml()
{
	return "<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>WebServer</title>\n\
</head>\n\
<body>\n\
		$2\n\
        $1\n\
</body>\n\
</html>\n\
";
}

std::string _getLink(std::string& dirname)
{
	std::stringstream ss;

	ss << "<p><a href=\"" << dirname + "\">" + dirname + "</a></p>\n\t\t";
	return ss.str();
}

int getPostLength(std::string data, std::string boundary)
{
	std::string _line = "";
	std::string body = "";
	std::istringstream _reader(data);
	int len = 0;
	bool _isEmpty = false;

	while (getline(_reader, _line))
	{
		if (!_isEmpty && _line.length() == 1)
			_isEmpty = true;
		else if (_isEmpty)
		{
			if (_line.length())
				len += _line.length() + 1;
		}
	}
	if (!boundary.length())
		len--;
	return (len);
}

int getLength(std::string data, std::string boundary)
{
	std::string _line = "";
	std::string body = "";
	std::istringstream _reader(data);
	bool _isEmpty = false;

	while (getline(_reader, _line))
	{
		if (!_isEmpty && _line.length() == 1)
			_isEmpty = true;
		else if (_isEmpty)
			body.append(_line).append("\n");
	}
	if (!boundary.length())
		body.pop_back();
	return (body.length());
}

int main()
{
	// DIR *dir;
	// struct dirent *pDirent;
	// std::string _data = "";
	// std::string _name = "";
	// std::string _body = "";
	// std::string _uri = "/";
	// std::string _line = "";
	// std::string _index = "<h1>Index of: " + _uri + "</h1>\n\t\t</pre><hr/>";

	// _body.append(_generateHtml());
	// _body.replace(_body.find("$2"), 2, _index);
	// // _body.append("<h1>Index of: " + _uri + "</h1>\n<hr />\n<pre>");
	// dir = opendir(_getCurrentDir().c_str());
	// if (dir)
	// {
	// 	while ((pDirent = readdir(dir)))
	// 	{
	// 		_name = pDirent->d_name;
	// 		if (pDirent->d_type == DT_DIR)
	// 			_name += "/";
	// 		_data.append(_getLink(_name));
	// 	}
	// }
	// // _data.pop_back();
	// _data.append("</pre><hr />");
	// _body.replace(_body.find("$1"), 2, _data);

	// std::ofstream file("test.html");
	// std::istringstream ss(_body);
	// while (getline(ss, _line))
	// {
	// 	file << _line;
	// 	file << std::endl;
	// }
	// std::cout << _getCurrentDir().back() << std::endl;
    std::string s[3] = {"/d/", "/dir/", "/"};
    std::string uri = "/";
	std::string u;
	std::string path = "";
	std::string l = uri.substr(0, uri.find_last_of("/"));

	/*std::cout << "l = " << l << "\n";
	if (uri.compare("/") == 0)
		u = uri;
	else if (uri.back() != '/' && _matchBegin(uri, l))
		u = uri;
	else if (uri.back() == '/')
		u = uri.substr(0, uri.find_last_of("/"));
	else
	{
		int j = 0;
		for (size_t i = 0; i < uri.size(); i++)
		{
			if (uri[i] == '/')
				j++;
		}
		if (j != 0 && j != 1)
			u = uri.substr(0, uri.find_last_of("/"));
		else
			u = uri;
	}
	std::cout << "u = " << u << std::endl;*/
    for (size_t i = 0; i < 3; i++)
    {
        //if (s[i].compare(u) == 0)
        //{
            // if (s[i].length() < s[i + 1].length())
                // std::cout << "s[i] = " << s[i] << "\n";
				//break ;
            // if (i)
        //}
		if (_matchBegin(s[i], uri))
		{
			if (s[i].compare(uri) == 0)
			{
				path = s[i];
				break ;
			}
			if (path.length() == 0)
				path = s[i];
			else if (path.length() < s[i].length())
				path = s[i];
		}
    }
	std::cout << "path = " << path  << "\n";

	std::string t = "\r\n";
    t += "----------------------------eb32ead89fa23a33\r\n";
    t += "Content-Disposition: form-data; name=\"fname\"\r\n";
    t += "\r\n";
    t += "Content-Type: text/plain\r\n";
    t += "ayoub\r\n";
    t += "----------------------------eb32ead89fa23a33\r\n";
    t += "Content-Disposition: form-data; filename=\"fname.txt\"\r\n";
    t += "\r\n";
    t += "Content-Type: text/ob-stream\r\n";
    t += "lodush=hayar\r\n";
    // t += "\r\n";
    t += "----------------------------eb32ead89fa23a33--\r\n";
	std::cout << "t.len = " << t.length() << std::endl;
	std::cout << "t.len1 = " << getPostLength(t, "a") << std::endl;
	std::cout << "t.len2 = " << getLength(t, "b") << std::endl;

	// std::ofstream ofp;
	// std::ifstream ifp;

	/*ifp.open("Viva la Vida by Coldplay lyrics - YouTube.mp4", std::ios::binary);
	ofp.open("myimage.mp4", std::ios::out);

	char buffer;

	if (ifp.is_open())
	{
		while (!ifp.eof())
		{
			ifp >> std::noskipws >> buffer;
			std::cout << buffer << std::endl;
			ofp << buffer;
		}
	}
	ifp.close();
	ofp.close();
	*/
}
