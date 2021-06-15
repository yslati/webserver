#ifndef CONVERTOR_HPP
# define CONVERTOR_HPP
# include <iostream>
namespace mlib {
class IConvertor {
    virtual int to_decimal(std::string const& x) const = 0;
};
};
#endif