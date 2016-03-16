// https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl

#include "foo.h"

template <typename T>
Foo<T>::Foo(T v)
   : val{v}
{}

template <typename T>
T Foo<T>::get() {
   return val;
}

template <typename T>
template<typename U>
void Foo<T>::set(U v) {
   val = v;
}

template class Foo<int>;
template void  Foo<int>::set<double>(double);
