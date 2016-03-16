#include <iostream>
#include "foo.h"

int main()
{
   Foo<int> foo{3};
   std::cout << foo.get() << std::endl;
   foo.set(4.1);
   std::cout << foo.get() << std::endl;
   return 0;
}


