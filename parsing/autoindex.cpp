

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
