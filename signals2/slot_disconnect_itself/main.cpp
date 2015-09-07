#include <iostream>
#include <boost/signals2.hpp>
#include <map>
#include "basewrapper.h"

using FuncSig  = void();

class MySig : BaseWrapper, public boost::signals2::signal<FuncSig> {
  // This class is just a boost::signals2::signal<FuncSig>
  // with a bit of extra-stuff for monitoring constructor/destructor behaviour (via BaseWrapper)
public:
  MySig() : BaseWrapper{"sig"}, boost::signals2::signal<FuncSig>{}
  {}
};

using Signal_t = MySig;

class Functor : public BaseWrapper {
  // Functor that we'll be using as a slot
public:
  Functor(std::map<const std::string, boost::signals2::connection> &map_conn_,
          const std::string &name_,
          bool destruct_conn_ = false)
    : BaseWrapper{name_}, map_conn{map_conn_}, destruct_conn{destruct_conn_}
  {
  }

  Functor(const Functor &other)
    : BaseWrapper{other}, map_conn{other.map_conn}, destruct_conn{other.destruct_conn}
  {
  }

  ~Functor()
  {
  }

  void operator()() {
    std::cout << "      >>>>>>>>>>>>>>>>>>>>>>\n"
              << "      (" << get_name() << ")         Functor::operator()()" << std::endl;
    if (destruct_conn) {
      auto it = map_conn.find(get_name());
      if (it != map_conn.cend()) {
        it->second.disconnect();
        map_conn.erase(it);
        std::cout << "                   Disconnected connection" << std::endl;
      } else {
        std::cout << "                   Connection already disconnected" << std::endl;
      }
    }
    std::cout << "      (" << get_name() << ") Done -- Functor::operator()()\n"
              << "      -----------------------" << std::endl;
  }

private:
  std::map<const std::string, boost::signals2::connection> &map_conn;
  bool destruct_conn;
};

int main()
{
  Signal_t sig;

  std::map<const std::string, boost::signals2::connection> map_conn;
  
#define DELETE_SIG_IN_FUNCTOR_SLOT
  
  {
    boost::signals2::connection conn;
    
    Functor f1{map_conn, "f1"};
    conn = sig.connect(f1);
    map_conn.emplace(f1.get_name(), conn);

    Functor f2{map_conn, "f2"};
    conn = sig.connect(f2);
    map_conn.emplace(f2.get_name(), conn);
    
    Functor f3{map_conn, "f3"
#ifdef DELETE_SIG_IN_FUNCTOR_SLOT
                          , true   // this functor-slot, should destruct the signal
#endif
              };
    conn = sig.connect(f3);
    map_conn.emplace(f3.get_name(), conn);

    Functor f4{map_conn, "f4"};
    conn = sig.connect(f4);
    map_conn.emplace(f4.get_name(), conn);

    Functor f5{map_conn, "f5"};
    conn = sig.connect(f5);
    map_conn.emplace(f5.get_name(), conn);


    // local functors go out of scope!
  }

  sig();

  std::cout << "End" << std::endl;
  return 0;
}
