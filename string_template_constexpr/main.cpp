#include <iostream>
#include <cassert>

//#define arr_len(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
template<typename T, std::size_t len> 
constexpr std::size_t arr_len(T(&)[len])
{
  return len;
}

//#define str_len(ARRAY) ((sizeof(ARRAY)/sizeof(ARRAY[0]))-1)
template<std::size_t len>
constexpr size_t str_len(const char(&)[len])
{
    return len-1;
}





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





template <const char *str_p> // identical with:      template <const char str_p[]>
class A {
public:
    void print() const { std::cout << str_p << " has c_str_len == " << len << std::endl;
    }
    constexpr const char *get_addr() const { return str_p; }
private:
    static constexpr std::size_t len = c_str_len(str_p);
};


extern constexpr const char name[] = "hello";



int main()
{
    A<name> a;
    a.print();
    static_assert(&name[0] == a.get_addr(), "");

    static constexpr const char root[] = "hello";
    static constexpr const char (&arr)[5] = "asdf";


    {
        C_STR_LEN("asdf");
        constexpr const char c_str[] = "asdf";
        C_STR_LEN(c_str);
        char str[] = "asdf";
        //C_STR_LEN(str);     // this would not compile! So we use C_STR_LEN to check that c_str_len is only called with a constexpr
    }
    
    return 0;
}
