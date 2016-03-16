#include <iostream>
#include <cassert>

class A {
public:
    A(A ** const ptr_to_this = nullptr) : ptr_to_this_{ptr_to_this}
    { }

    ~A() {
        if (ptr_to_this_) {
            *ptr_to_this_ = nullptr;
        }
    }
private:
    A ** const ptr_to_this_;
};

int main()
{
    {
        A *p = new A();
        assert(p);
        delete p;
        assert(p);
    }
    
    {
        A *p = new A(&p);
        assert(p);
        delete p;
        assert(p == nullptr); // !
    }
    return 0;
}
