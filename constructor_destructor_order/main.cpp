#include <iostream>

struct B1 {
  B1()  { std::cout << "B1()" << std::endl; }
  virtual ~B1() { std::cout << "~B1()" << std::endl; }
};

struct B2 {
  B2()  { std::cout << "B2()" << std::endl; }
  virtual ~B2() { std::cout << "~B2()" << std::endl; }
};


struct A1 {
  A1()  { std::cout << "A1()" << std::endl; }
  ~A1() { std::cout << "~A1()" << std::endl; }
};


struct A2 {
  A2()  { std::cout << "A2()" << std::endl; }
  ~A2() { std::cout << "~A2()" << std::endl; }
};

struct D : public B1, public B2 {
  D() : a1{} { std::cout << "D()" << std::endl; }
  ~D(     ) { std::cout << "~D()" << std::endl; }

private:
  A1 a1;
  A2 a2;
};


int main()
{
  {
    D d;
  }

  std::cout << '\n' << std::endl;
  
  {
    D  *d = new D;
    B1 *x = d;
    delete x; // delete through one of the base classes  (need virtual on both base classes)
  }

  std::cout << '\n' << std::endl;
  
  {
    D  *d = new D;
    B2 *x = d;
    delete x; // delete through other base class  (need virtual on both base classes)
  }

  
  return 0;
}
