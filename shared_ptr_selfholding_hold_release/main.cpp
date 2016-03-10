#include <iostream>
#include <memory>
#include <cassert>

class Base {
public:
    Base(int a) : a_{a}
    {
        std::cout << "Ctor" << std::endl;
    }
    ~Base()
    {
        std::cout << "Dtor" << std::endl;
    }

    virtual void hold()    {}
    virtual void release() {}

private:
    int a_;
};


class Deriv : public std::enable_shared_from_this<Deriv>, public Base {
public:
    Deriv(int a, int b) : Base{a}, b_{b}
    {
    }
    
    void hold() override {
        sp = shared_from_this();
        Base::hold();
    }

    void release() override {
        sp.reset();
        Base::release();
    }

private:
    int b_;
    std::shared_ptr<Deriv> sp;
};

int main()
{
    {
        std::shared_ptr<Deriv> sp = std::make_shared<Deriv>(1, 2);
        auto x = sp->shared_from_this();
        assert(x == sp);
    }
    std::cout << std::endl;
    
    std::weak_ptr<Base> wp;
    {
        std::shared_ptr<Base> sp = std::make_shared<Deriv>(1, 2); // working from base class in this case
        sp->hold();
        wp = sp;
    }
    if (auto spt = wp.lock()) {
        spt->release(); // comment/uncomment this line and observe the behaviour!!
    }

    
    return 0;
}
