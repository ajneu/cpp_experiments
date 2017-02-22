#include <iostream>
#include <cassert>
#include <sstream>

// http://stackoverflow.com/a/8765699
struct membuf: public std::streambuf {
  template <size_t Size> membuf(char (&array)[Size]) {
    this->setp(array, array + Size - 1);

    assert( array           == pbase());
    assert((array + Size-1) == epptr());

    *pbase() = '\0';
    *epptr() = '\0'; // std::fill_n(array, Size, 0); 
  }
};

// http://stackoverflow.com/a/8765699 (also)
struct omemstream: virtual membuf, std::ostream {
  template <size_t Size> omemstream(char (&array)[Size]):
    membuf(array),
    std::ostream(this)
  {
  }

  void rewind() {
    this->clear();
    this->setp(pbase(), epptr());
    *pbase() = '\0';
  }

  void ready_for_append() {
    if ((pptr() > pbase()) && (*(pptr()-1) == '\0')) {
      pbump(-1);
    }
  }
};

int main()
{
  char buf[5] = "ok";

  omemstream out(buf);
  std::cout << buf << std::endl;
  
  out << "ab" << std::ends;
  std::cout << buf << std::endl;

  out.ready_for_append();
  out << "cd" << std::ends;
  std::cout << buf << std::endl;

  out.ready_for_append();
  out << "ef" << std::ends; // cannot append as buf is already full
  std::cout << buf << std::endl;

  out.rewind(); //======================
  std::cout << buf << std::endl;

  out << "AB" << std::ends;
  std::cout << buf << std::endl;

  out.ready_for_append();
  out << "CDEFGHIJ" << std::ends;
  std::cout << buf << std::endl;

  return 0;
}
