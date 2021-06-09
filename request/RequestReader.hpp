#ifndef REQUEST_READER_HPP
# define REQUEST_READER_HPP
# include "IRequestReader.hpp"
# include <sys/socket.h>
# include <iostream>

class RequestReader: public  IRequestReader {
    public:
        virtual void readConnection(int);
    private:
};
#endif