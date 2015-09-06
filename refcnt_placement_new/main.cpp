#include <iostream>
#include "basewrapper.h"


template<typename T>
class PlaceConstr {
public:
  using buf_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

  T &operator*()  const { return *reinterpret_cast<T*>(buf); }
  T *operator->() const { return  reinterpret_cast<T*>(buf); }

  bool is_valid() const { return *valid; }

  void destruct()
  {
    if (*valid) {
      reinterpret_cast<T*>(buf)->~T();
      *valid = false;
    }
  }

protected:  
  template <typename... Params>
  PlaceConstr(buf_t *buf_, bool *vali, Params&&... args)
    : buf{buf_}, valid{((::new (buf) T(std::forward<Params>(args)...)), *vali = true, vali)}
  { }

  PlaceConstr(const PlaceConstr &other) : buf{other.buf}, valid{other.valid} { }

  virtual ~PlaceConstr()
  {
  }
  
private:
  buf_t *buf;
  bool  *valid;
};


template <typename T>
class RefCountPlace : public PlaceConstr<T> {
public:
  template <typename... Params>
  RefCountPlace(size_t *cnt_, typename PlaceConstr<T>::buf_t *buf, bool *valid, Params... args)
    : PlaceConstr<T>(buf, valid, std::forward<Params>(args)...), cnt_p{(*cnt_ = 1U, cnt_)}
  { }
  
  RefCountPlace(const RefCountPlace &other)
    : PlaceConstr<T>{other}, cnt_p{&(++(*other.cnt_p))}
  { }

  ~RefCountPlace()
  {
    if (--*cnt_p == 0) {
      PlaceConstr<T>::destruct(); /* May already have been destructed by user-call to destruct().
                                     destruct() checks that */
    }
  }
  
private:
  size_t *cnt_p;
};





void print(RefCountPlace<BaseWrapper> obj) // obviously by ref would be better, but we're exercising the copy constructor here
{
  if (obj.is_valid()) {
    std::cout << "name: " << (*obj).get_name() << std::endl;
  }
}

void destr(RefCountPlace<BaseWrapper> obj) // obviously by ref would be better, but we're exercising the copy constructor here
{
  obj.destruct();
}

#define CMD(x) std::cout << "command: " #x << std::endl; x

int main()
{
    size_t cnt;
    typename PlaceConstr<BaseWrapper>::buf_t buf;
    bool valid;

    CMD(RefCountPlace<BaseWrapper> obj1(&cnt, &buf, &valid,
                                       // constructor arguments of template (here: BaseWrapper)
                                       "asdf");
       );
    CMD(print(obj1));
    CMD(RefCountPlace<BaseWrapper> obj2 = obj1);
    CMD(print(obj2));

    CMD(destr(obj2));
    CMD(print(obj1));

    CMD(destr(obj1)); // already destructed (no effect)
    CMD(print(obj1));

    return 0;
}
