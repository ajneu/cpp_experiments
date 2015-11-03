#include <iostream>
#include <memory>
#include <experimental/optional>
#include "basewrapper.h"

template<typename T>
class PlaceStackConstr {
public:
  template <typename... Params>
  PlaceStackConstr(Params&&... args) : is_destructed{(::new (&buf) T(std::forward<Params>(args)...), false)} { }
  ~PlaceStackConstr() { destruct(); }
  
  T &operator*()  { return *reinterpret_cast<T*>(&buf); }
  T *operator->() { return  reinterpret_cast<T*>(&buf); }
  void destruct()
  {
    if (!is_destructed) {
      reinterpret_cast<T*>(&buf)->~T();
      is_destructed = true;
    }
  }
private:
  using buf_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
  buf_t buf;      //alignas(alignof(T)) unsigned char buf[sizeof(T)];
  bool is_destructed;
};

class RefCount1 {
public:
  RefCount1() : cnt_p{new decltype(*cnt_p+0){1U}} {}
  RefCount1(const RefCount1 &other) : cnt_p{&(++(*other.cnt_p))} {}
  ~RefCount1()
  {
    if (--*cnt_p == 0) {
      delete cnt_p;
      std::cout << "delete" << std::endl;
    }
  }
private:
  size_t *cnt_p;
};

class RefCountPlace{
public:
  RefCountPlace(size_t *p) : cnt_p{(*p = 1U, p)} {}
  RefCountPlace(const RefCountPlace &other) : cnt_p{&(++(*other.cnt_p))} {}
  ~RefCountPlace()
  {
    if (--*cnt_p == 0) {
      std::cout << "delete" << std::endl;
    }
  }
private:
  size_t *cnt_p;
};


template<typename T>
inline bool inner_unique_ptr_valid(const std::shared_ptr<std::unique_ptr<T>> &p)
{
  return (p    // shared_pointer holding an actual pointer
          &&
          *p); // unique_pointer holding an actual pointer
}

void reset_func(std::shared_ptr<std::unique_ptr<BaseWrapper>> b)
{
  if (b) {
    (*b).reset();
  }
}

void print_func(std::shared_ptr<std::unique_ptr<BaseWrapper>> b)
{
  if (inner_unique_ptr_valid(b)) {
    std::cout << (**b).get_data() << std::endl;
  }

  reset_func(b);

  if (inner_unique_ptr_valid(b)) {
    std::cout << (**b).get_data() << std::endl;
  } else {
    std::cout << "been deleted" << std::endl;
  }
}

int main()
{
  {
    BaseWrapper b1{"b1"};
    // destructor when b1 goes out of scope
  }

  std::cout << '\n' << std::endl;
  
  {
    BaseWrapper *b2 = new BaseWrapper{"b2"};

    delete b2; // destructor when delete is called
  }

  std::cout << '\n' << std::endl;
  
  {
    std::shared_ptr<BaseWrapper> b3_a;
    {
      std::shared_ptr<BaseWrapper> b3_b{new BaseWrapper{"b3"}};
      b3_a = b3_b;
    }
    std::cout << "p is out of scope" << std::endl;
    // destructor called when the last shared_ptr releases the resource
  }

  std::cout << '\n' << std::endl;

  {
    std::unique_ptr<BaseWrapper> b4_a{new BaseWrapper{"b4"}};
    {
      std::unique_ptr<BaseWrapper> b4_b = std::move(b4_a); // resource held uniquely by a single unique_ptr
    }
    std::cout << "b4_b out of scope" << std::endl;
  }

  std::cout << '\n' << std::endl;

  {
    std::shared_ptr<std::unique_ptr<BaseWrapper>> b5 = std::make_shared<std::unique_ptr<BaseWrapper>>(new BaseWrapper{"b5"});
    print_func(b5);
  }
  
  std::cout << '\n' << std::endl;

  {
    std::experimental::optional<BaseWrapper> b6 (std::experimental::in_place,
                                                 "b6");
    b6 = std::experimental::nullopt; // call destructor exactly here
    std::cout << "b6 not yet out of scope, but already destructed" << std::endl;
  }

  std::cout << '\n' << std::endl;
  
  {
    PlaceStackConstr<BaseWrapper> b7("b7");
    std::cout << (*b7).get_data() << std::endl;
    std::cout << b7->get_data() << std::endl;
    b7.destruct();
    std::cout << "destructed (but b7 not yet out of scope)" << std::endl;
  }

  std::cout << '\n' << std::endl;
  
  {
    RefCount1 r;
    RefCount1 r2 = r;
    {
      RefCount1 r3 = r2;
    }
  }

  std::cout << '\n' << std::endl;
  
  {
    size_t x;
    RefCountPlace r(&x);
    RefCountPlace r2 = r;
    {
      RefCountPlace r3 = r2;
    }
  }

  return 0;
}
