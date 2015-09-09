#include "basewrapper.h"

BaseWrapper::BaseWrapper(const std::string &name_)
  : name{name_}
{
  std::cout << "#constructor      \t" << name << " (" << ++mm[this] << ')' << std::endl;    
}

BaseWrapper::BaseWrapper(const BaseWrapper &other)
  : name{other.name}
{
  std::cout << "#copy-constructor \t" << name << " (" << ++mm[this] << ')' << std::endl;
}

BaseWrapper::~BaseWrapper()
{
  std::cout << "#destructor       \t" << name << " (" << (mm[this])-- << ')' << std::endl;
}



std::map<const BaseWrapper* const, int> BaseWrapper::mm;
