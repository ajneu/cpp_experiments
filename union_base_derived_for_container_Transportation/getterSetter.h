#ifndef GETTERSETTER_H
#define GETTERSETTER_H

template<typename T>
class GetterSetter {
public:
    using Type = T;
    
    GetterSetter() = default;
    GetterSetter(const GetterSetter &rhs) : t_{rhs.t_} {}
    GetterSetter(const T &t) : t_{t} {}

    T get() const      { return t_; }
    operator T() const { return t_; }
    
    void set(const T &t)     {        t_ = t; }
    T &operator=(const T &t) { return t_ = t; }
private:
    T t_{};
};

#endif
