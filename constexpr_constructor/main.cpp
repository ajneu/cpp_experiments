#include <iostream>
#include <string>
#include <cassert>

class A {
public:
    A(int i) : i_(i) {
    }
    int i_;
};

class AA {
public:
    constexpr AA(int i) : i_(i) {
    }
    int i_;
};


extern const A   a;
extern const AA aa;

std::string str_a(  a.i_, 'x');
std::string str_aa(aa.i_, 'x');

const A   a(4); // is not in ro section!
const AA aa(4); // in in     ro section!

const char global_ro_text[] = "asdf";
char       global_wr_text[] = "qwer";

int main()
{
    std::cout << "str_a  == "  << str_a  << std::endl;
    std::cout << "str_aa == " << str_aa << std::endl;

    std::cout << "&a.i_  == " << &a.i_ << std::endl;
    std::cout << "&aa.i_ == " << &aa.i_ << std::endl;
    std::cout << std::endl;
    
    std::cout << "&global_ro_text == " << &global_ro_text << std::endl;
    assert(static_cast<const void*>(&global_ro_text) == static_cast<const void*>(&global_ro_text[0]));

    std::cout << "&global_wr_text == " << &global_wr_text << std::endl;

    return 0;
}
