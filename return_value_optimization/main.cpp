#include <iostream>


class Member {
public:
   Member(int v = 1) : val{v}  {
      std::cout << "ctor_Member \t(default)" << std::endl;
   }
   Member(const Member &rhs) : val{rhs.val} {
      std::cout << "ctor_Member \t(copy)" << std::endl;
   }
   ~Member() {
      std::cout << "dtor_Member" << std::endl;
   }

   int val;
};


class A {
public:
   A(int v = 1) : mem{v}  {
      std::cout << "ctor_A   \t(default)" << std::endl;
   }
   A(const A &rhs) : mem{rhs.mem} {
      std::cout << "ctor_A   \t(copy)" << std::endl;
   }
   ~A() {
      std::cout << "dtor_A" << std::endl;
   }

   A &operator=(const A &rhs) {
      mem.val = rhs.mem.val;
      std::cout << "operator=_A" << std::endl;
   }
   Member mem;
};


// uncomment one of the following
#define RETURN_VALUE_OPTIMIZATION
//#undef  RETURN_VALUE_OPTIMIZATION

#ifdef RETURN_VALUE_OPTIMIZATION
A get(int i) {
   A tmp{0};
   if (i % 2 == 0)
      return tmp;

   tmp.mem.val = 1;
   return tmp;

   // http://www.informit.com/articles/article.aspx?p=25033&seqNum=3
   // The language extension never became part of the language, but the optimization itself has become nearly universal.
   // It is called the name RETURN VALUE OPTIMIZATION.
   // It is applied implicitly by a compiler when the same named object is returned from all the exit points of a function. 
}


// // It also works when a temporary of the same type is returned from all exit points of a function
// A get(int i) {
//    if (i % 2 == 0)
//       return A{0};

//    return A{1};
// }


#else
A get(int i) {
   A tmp{0};
   if (i % 2 == 0)
      return tmp;

   A tmp1{1};
   return tmp1;
}

#endif


A get_fast(int i) {
   return A{i%2};
}



int main()
{
   A a = get(0);

   std::cout << std::endl;

   // //uncomment one of the following
#define QUICK
//#undef QUICK

#ifdef QUICK
   {
      for (int i = 0; i < 2; ++i) {
	 A a = get_fast(i);           // return value optimization ! (instead of using a temporary... the actual object a is used)
      }
      // http://www.informit.com/articles/article.aspx?p=25033&seqNum=4
   }
#else
   {
      A a;  // programmers think it's fast to just construct this once (instead of on every loop)
      for (int i = 0; i < 2; ++i) {
	 a = get_fast(i);  // in actual fact this line is incredibly slow: constructor and destructor of a temporary, as well as copy-assignment!!
      }
   }
#endif
   return 0;
}
