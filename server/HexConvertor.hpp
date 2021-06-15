#ifndef HEX_CONVERTOR_HPP
# define HEX_CONVERTOR_HPP
# include "IConvertor.hpp"
# include <cmath>

namespace mlib {
class HexConvertor : public IConvertor {
    public:
        virtual int to_decimal(std::string const& x) const;
    private:
};
};
#endif