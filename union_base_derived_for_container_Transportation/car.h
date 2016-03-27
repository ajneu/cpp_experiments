#ifndef CAR_H
#define CAR_H

#include "speed.h"
#include "name.h"
#include "transportation.h"

extern constexpr char car_name[] = "Car: ";

class Car : public Speed, public Name<car_name, 20> {
public:
    Car(const char *name = "a car", unsigned speed = 0) : Speed{speed}, Name{name} {
    }
    Car(const Car &rhs) : Speed{static_cast<const Speed&>(rhs)}, Name{rhs.name()} {}

    void move_forward() const {
        std::cout << "drive" << std::endl;
    }

    static constexpr TransEnum id() { return TransEnum::car; } // enum in header transportation.h
};

#endif
