#include <iostream>
#include <boost/signals2.hpp>
#include <map>
#include <memory>

class BaseWrapper {
  // Class used for monitoring constructor and destructor behaviour.
  // Other classes just need to derive from this class!
public:
  BaseWrapper(const std::string &name_)
    : name{name_}
  {
    std::cout << "#constructor      \t" << name << " (" << ++mm[name] << ')' << std::endl;    
  }

  BaseWrapper(const BaseWrapper &other)
    : name{other.name}
  {
    std::cout << "#copy-constructor \t" << name << " (" << ++mm[name] << ')' << std::endl;
  }

  virtual ~BaseWrapper()
  {
    std::cout << "#destructor       \t" << name << " (" << (mm[name])-- << ')' << std::endl;
  }

  const std::string &get_name() const {return name; };
private:
  static std::map<const std::string, int> mm;
  const std::string name;
};

std::map<const std::string, int> BaseWrapper::mm;
  



using FuncSig  = void();

class MySig : BaseWrapper, public boost::signals2::signal<FuncSig> {
  // This class is just a boost::signals2::signal<FuncSig>
  // with a bit of extra-stuff for monitoring constructor/destructor behaviour (via BaseWrapper)
public:
  MySig() : BaseWrapper{"sig"}, boost::signals2::signal<FuncSig>{}
  {}
};

using Signal_t = MySig;

template<typename T>
inline bool inner_unique_ptr_valid(const std::shared_ptr<std::unique_ptr<T>> &p)
{
  return (p    // shared_pointer holding an actual pointer
          &&
          *p); // unique_pointer holding an actual pointer
}

class Functor : BaseWrapper {
  // Functor that we'll be using as a slot
public:
  Functor(std::shared_ptr<std::unique_ptr<Signal_t>> sig_p_, const std::string &name_, bool destruct_sig_ = false)
    : BaseWrapper{name_}, sig_p{sig_p_}, destruct_sig{destruct_sig_}
  {
  }

  Functor(const Functor &other)
    : BaseWrapper{other}, sig_p{other.sig_p}, destruct_sig{other.destruct_sig}
  {
  }

  ~Functor()
  {
  }

  void operator()() {
    std::cout << "      >>>>>>>>>>>>>>>>>>>>>>\n"
              << "      (" << get_name() << ")         Functor::operator()()" << std::endl;
    if (destruct_sig) {
      if (inner_unique_ptr_valid(sig_p)) {
        (*sig_p).reset(); // reset unique_ptr    (it's content is replaced with nullptr, and the previous ptr-content deleted)
        std::cout << "                   Deleted sig" << std::endl;
      } else {
        std::cout << "                   sig already deleted" << std::endl;
      }
    }
    std::cout << "      (" << get_name() << ") Done -- Functor::operator()()\n"
              << "      -----------------------" << std::endl;
  }

private:
  std::shared_ptr<std::unique_ptr<Signal_t>> sig_p;
  bool destruct_sig;
};



int main()
{
  std::shared_ptr<std::unique_ptr<Signal_t>> sig_p = std::make_shared<std::unique_ptr<Signal_t>>(new Signal_t);

  /* Background on the above type:
     
     This should behave like shared_ptr, but one whose content can be globally destructed from anywhere.
     Anybody still having access, needs to check if it has been destructed.
     If nobody has access anymore, it needs to destruct itself.

     // The usage is thus:
     if (inner_unique_ptr_valid(sig_p)) { // check if still valid
        (**sig_p).member();               // access
     }
     // The two steps above would need to be mutex-protected in a multithreaded environment


     // To globally destruct:
     if (sig_p) {                         // check that shared_ptr is actually holding a pointer (in this case a unique_ptr)
        (*sig_p).reset();                 // reset the unique_ptr
     }

   */


#define DELETE_SIG_IN_FUNCTOR_SLOT
  
  {
    Functor f1{sig_p, "f1"};
    if (inner_unique_ptr_valid(sig_p)) {
      (*sig_p)->connect(f1);   // connect to slot (functor)
    }

    Functor f2{sig_p, "f2"};
    if (inner_unique_ptr_valid(sig_p)) {
      (*sig_p)->connect(f2);   // connect to slot (functor)
    }

    Functor f3{sig_p, "f3"
#ifdef DELETE_SIG_IN_FUNCTOR_SLOT
                          , true   // this functor-slot, should destruct the signal
#endif
              };
    if (inner_unique_ptr_valid(sig_p)) {
      (*sig_p)->connect(f3);   // connect to slot (functor)
    }

    Functor f4{sig_p, "f4"};
    if (inner_unique_ptr_valid(sig_p)) {
      (*sig_p)->connect(f4);   // connect to slot (functor)
    }

    Functor f5{sig_p, "f5"};
    if (inner_unique_ptr_valid(sig_p)) {
      (*sig_p)->connect(f5);   // connect to slot (functor)
    }

    // local functors go out of scope!
  }

  
  if (inner_unique_ptr_valid(sig_p)) {
    (**sig_p)();    // emit signal
    std::cout << "signal emit - Finished" << std::endl;
  }

  if (inner_unique_ptr_valid(sig_p)) {
    (*sig_p).reset();
    std::cout << "sig deleted in main(); not in functor" << std::endl;
  }

  std::cout << "End" << std::endl;
  return 0;
}
