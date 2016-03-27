#ifndef CONSTEXPR_H
#define CONSTEXPR_H

template<typename T, T val>
struct TestConstexpr {
    static constexpr T c_val = val;
};

#define TEST_CONSTEXPR(var) TestConstexpr<std::decay_t<decltype(var)>, var>()

// c_str_len can be called from a constexpr string (e.g. literal), but also from a non-constexpr string
// To ensure that it is only called from a constexpr... see the macro C_STR_LEN below
constexpr std::size_t c_str_len(const char *str, std::size_t len = 0)
{
    return (str[0] == '\0')
            ? len
            : c_str_len(str+1, len+1);
}

#define C_STR_LEN(STR) (TEST_CONSTEXPR(c_str_len(STR)), c_str_len(STR));

#endif
