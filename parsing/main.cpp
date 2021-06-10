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

int main()
{
	try {
		pars ser("./parsing/file.conf");
	} catch (char const *e) {
		std::cout << e << std::endl;
	}

	// Server& srv = Server::getInstance();

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
	return 0;
}
