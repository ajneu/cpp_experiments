// https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl

#ifndef FOO_H
#define FOO_H

template <typename T>
class Foo {
private:
   T val;
public:
   Foo(T v);

   T get();

   template<typename U>
   void set(U v);
};

#endif
