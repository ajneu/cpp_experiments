#ifndef SPEED_H
#define SPEED_H

#include "getterSetter.h"

class Speed {
public:
    Speed(unsigned speed)   : speed_{speed} {}
    Speed(const Speed &rhs) : speed_{rhs.speed_} {}
    
    unsigned speed() const {
        return speed_;  // return speed_.get();
    }
    void set_speed(unsigned speed) {
        speed_ = speed; // return speed_.set(speed);
    }
private:
    GetterSetter<unsigned> speed_; // unsigned speed_;
};

#endif
