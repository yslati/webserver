#include "HexConvertor.hpp"

int mlib::HexConvertor::to_decimal(std::string const& x) const {
    int power = 0;
    int res = 0;
    for (int i = x.size() - 1; i >= 0; i--) {
        int digit;
        if (x[i] >= '0' && x[i] <= '9') {
            digit = x[i] - 48;
        } else if (x[i] >= 'A' && x[i] <= 'F') {
            digit = x[i] - 55;
        }
        res += digit * std::pow(16, power);
        power++;
    }
    
    return (res);
}