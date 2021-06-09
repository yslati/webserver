#ifndef IREQUEST_READER_HPP
# define IREQUEST_READER_HPP
class IRequestReader {
    public:
        virtual void readConnection(int) = 0;
};
#endif