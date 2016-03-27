#ifndef NAME_H
#define NAME_H

#include "constexpr.h"
#include <cstring>

// class Name holds a single string buffer_,
//   whose initial part is a fixed copy of StrInitial
//   followed by a variable part
template<const char *StrInitial, std::size_t Len>
class Name {
public:
    Name(const char *str_variable) {
        static_assert((len_strinitial+1) <= Len, "err");
        std::memcpy(buffer_, StrInitial, len_strinitial+1); // memcpy is fast: http://en.cppreference.com/w/cpp/string/byte/memcpy#Notes
        set_name(str_variable);
    }
    Name(const Name &rhs) {
        static_assert((len_strinitial+1) <= Len, "err");
        std::memcpy(buffer_, StrInitial, len_strinitial+1); // memcpy is fast: http://en.cppreference.com/w/cpp/string/byte/memcpy#Notes
        strcpy(buffer_+len_strinitial, rhs.name()+len_strinitial); // strncat not needed, since type Name already guarantees a SUITABLE ending '\0'
    }

    const char *name() const { return buffer_; }

    void set_name(const char *str_variable) {
        char *var_part = buffer_+len_strinitial;
        var_part[0] = '\0';

        strncat(var_part, str_variable, (Len-1-len_strinitial)); // properly truncate, in case str is too long
    }
private:
    static constexpr std::size_t len_strinitial = C_STR_LEN(StrInitial);
    char buffer_[Len];
};


#endif
