#ifndef CHUNKED_HANDLER_HPP
# define CHUNKED_HANDLER_HPP
# include <iostream>

class ChunkedHandler {
    public:
        static void  handle(std::string& content);
};

#endif