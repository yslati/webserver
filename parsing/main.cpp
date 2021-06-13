#include "parsing.hpp"

// std::vector<std::string> _split(std::string const &line)
// {
//     std::vector<std::string> wordsArr;
//     std::stringstream ss(line);
//     std::string buff;

//     while (ss >> buff)
//         wordsArr.push_back(buff);
//     return wordsArr;
// }

// void	allMethod(std::string str) {
// 	char tmp[str.size()];
// 	for (int i = 0; i < str.size(); i++) {
// 		tmp[i] = str[i];
// 	}
// 	char * pch = strtok(tmp, ",");
// 	while (pch != NULL)
//   {
// 		printf ("%s\n",pch);
// 		pch = strtok (NULL, ",");
//   }
// }

// std::string         getLink(std::string const &dirName) {
//     std::stringstream   ss;
//     ss << "<p><a href=\"" << dirName + "\">" + dirName + "</a></p>";
//     return ss.str();
// }

// std::string getHtmlSkeleton()
// {
//     return "<!DOCTYPE html>\
//     <html lang=\"en\">\
//     <head>\
//         <meta charset=\"UTF-8\">\
//         <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
//         <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
//         <title>Http dial 3bar</title>\
//     </head>\
//     <body>\
// 		<hr><pre>\
//         $1\
// 		</pre><hr>\
//     </body>\
//     </html>";
// }

// bool isDirectory(const std::string &s)
// {
//     DIR *dir;
// 	std::string name;
// 	struct dirent *ent;
// 	std::string links;

//     if ((dir = opendir(s.c_str())) == NULL)
//         return 0;
// 	while ((ent = readdir (dir)) != NULL) {
// 		name = ent->d_name;
// 		if (ent->d_type == DT_DIR)
// 			name.append("/");
// 		links.append(getLink(name));
// 	}
// 	std::cout << getHtmlSkeleton().replace(getHtmlSkeleton().find("$1"), 2, links);
// 	closedir (dir);
//     return 1;
// }

int main()
{
	try {
		pars ser("./parsing/file.conf");
	} catch (char const *e) {
		std::cout << e << std::endl;
	}
// int main()
// {
// 	try {
// 		pars ser("./parsing/file.conf");
// 	} catch (char const *e) {
// 		std::cout << e << std::endl;
// 	}

// 	// Server& srv = Server::getInstance();

	// for(int i = 0; i < srv.getHttpServers().size(); i++) {
	// 	std::cout << "Server " << i + 1 << std::endl;
	// 	std::cout << "Methods Size: " << srv.getHttpServers()[i]._allowed_methods.size() << std::endl;
	// 	std::cout << "Location Size: " << srv.getHttpServers()[i]._locations.size() << std::endl;
	// } 
	// allMethod("GET,PUT,HEAD,POST");
	// 	std::vector<std::string> arr=_split("GET	 	 	 	PUT 	 	 HEAD POST    Something");
	// for (size_t i = 0; i < arr.size(); i++)
	// {
	// 	std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
	// }

	// std::cout << isDirectory("./parsing") << std::endl;
	return 0;
}
// 	// for(int i = 0; i < srv.getHttpServers().size(); i++) {
// 	// 	std::cout << "Server " << i + 1 << std::endl;
// 	// 	std::cout << "Methods Size: " << srv.getHttpServers()[i]._allowed_methods.size() << std::endl;
// 	// 	std::cout << "Location Size: " << srv.getHttpServers()[i]._locations.size() << std::endl;
// 	// } 
// 	// allMethod("GET,PUT,HEAD,POST");
// 	// 	std::vector<std::string> arr=_split("GET	 	 	 	PUT 	 	 HEAD POST    Something");
// 	// for (size_t i = 0; i < arr.size(); i++)
// 	// {
// 	// 	std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
// 	// }
// 	return 0;
// }
