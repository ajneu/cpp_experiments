#ifndef PLANE_H
#define PLANE_H

#include "speed.h"
#include "name.h"

extern constexpr char plane_name[] = "Airoplane: ";

class Plane : public Speed, public Name<plane_name, 30> {
public:
    Plane(const char *name = "a plane", unsigned speed = 0) : Speed{speed}, Name{name} {
    }
    Plane(const Plane &rhs) : Speed{static_cast<const Speed&>(rhs)}, Name{rhs.name()} {}
    
    void move_forward() const {
        std::cout << "fly" << std::endl;
    }

    static constexpr TransEnum id() { return TransEnum::plane; } // enum in header transportation.h
};

#endif
