#ifndef BASEWRAPPER_H
#define BASEWRAPPER_H

#include <iostream>
#include <string>
#include <map>

class BaseWrapper {
  // Class used for monitoring constructor and destructor behaviour.
  // Other classes just need to derive from this class!
public:
  BaseWrapper(const std::string &name_);
  BaseWrapper(const BaseWrapper &other);
  virtual ~BaseWrapper();

  const std::string &get_name() const {return name; };
private:
  static std::map<const BaseWrapper* const, int> mm;
  const std::string name;
};


#endif
