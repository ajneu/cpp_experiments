#include "basewrapper.h"

BaseWrapper::BaseWrapper(const std::string &name_)
  : name{name_}
{
  std::cout << "#constructor      \t" << name << " (" << ++mm[name] << ')' << std::endl;    
}

BaseWrapper::BaseWrapper(const BaseWrapper &other)
  : name{other.name}
{
  std::cout << "#copy-constructor \t" << name << " (" << ++mm[name] << ')' << std::endl;
}

BaseWrapper::~BaseWrapper()
{
  std::cout << "#destructor       \t" << name << " (" << (mm[name])-- << ')' << std::endl;
}



std::map<const std::string, int> BaseWrapper::mm;
