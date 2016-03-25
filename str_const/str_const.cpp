#include <iostream>

constexpr bool c_str_same(char const* str_a, char const* str_b)
{
    return ((str_a[0] == str_b[0])
            ? ((str_a[0] == '\0')
               ? true
               : c_str_same(str_a+1, str_b+1))
            : false);
}


// code on page 29 https://github.com/boostcon/cppnow_presentations_2012/blob/master/wed/schurr_cpp11_tools_for_class_authors.pdf?raw=true
class str_const {                                // constexpr string
private:
    const char* const p_;
    const std::size_t sz_;
public:
    template<std::size_t N>
    constexpr str_const(const char(&a)[N]) :     // ctor
        p_(a), sz_(N-1) {}
    constexpr str_const(const str_const &str, std::size_t offset) :
        p_(str.p_+offset), sz_(str.sz_-offset) {}
    constexpr char operator[](std::size_t n) {   // []
        return n < sz_ ? p_[n] :
            throw std::out_of_range("");
    }
    constexpr std::size_t size() { return sz_; } // size()
    constexpr bool eq(const str_const &rhs) {     // the following conflicts: constexpr bool operator==(const str_const &rhs) {
        return c_str_same(p_, rhs.p_);
    }

    constexpr operator const char*() {
        return p_;
    }

};

int main()
{
    // http://stackoverflow.com/a/15863826
    constexpr str_const my_string = "Hello, world!";
    static_assert(my_string.size() == 13, "");
    static_assert(my_string[4] == 'o', "");
    constexpr str_const my_other_string = my_string;
    static_assert(my_string.eq(my_other_string), "");
    constexpr str_const world(my_string, 7);
    static_assert(world.eq("world!"), "");
    std::cout << world << std::endl;


    // // this ain't working ...
    //constexpr str_const world(my_string, 7, 5);
    //    static_assert(world == "world", "");
}
