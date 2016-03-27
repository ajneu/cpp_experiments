Having a container of a base-type.
==================================

Usually you'll do something like this:
   std::array<std::unique_ptr<Base>, 3> arr{{std::make_unique<Derived1>("constructor args1"), std::make_unique<Derived2>("constructor args2")}};
where Base is a virtual abstract base class.    

For no usage of the free-store (heap), you could use something like this:
  Derived1 deriv1{"constructor args1"};
  Derived2 deriv2{"constructor args2"};
  std::array<Base *, 2> arr{{&deriv1, &deriv2}};

If additionally we want all the memory in the container itself, we need some kind of union. Usually it's best to use a variant. But here we'll explore using a union.
  Check out the code in tranu.h.
  Basically we're using placement new.
