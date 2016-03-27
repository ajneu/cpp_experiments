#ifndef TRANSU_H
#define TRANSU_H

#include <cassert>
#include "transportation.h"
#include "car.h"
#include "plane.h"

union TransU {
public:
    // constructors
    TransU(const Car &car) {
        new (&car_) TransportationDerived<Car>(car);
    }
    TransU(const Plane &plane) {
        new (&plane_) TransportationDerived<Plane>(plane);
    }
    TransU(TransEnum id, const char *name = "a transportation mechanism", unsigned speed = 0) {
        switch(id) {
        case TransEnum::car:
            new (&car_) TransportationDerived<Car>(name, speed);
            break;
        case TransEnum::plane:
            new (&plane_) TransportationDerived<Plane>(name, speed);
            break;
        default:
            assert(false);
        }
    }
    TransU(const TransU &rhs) : TransU(rhs.base().id(), rhs.base().name(), rhs.base().speed()){
    }
    

    // destructor
    ~TransU() {
        base().~TransportationBase();
    }

    TransportationBase       &base()       { return reinterpret_cast<TransportationBase&>(*this); }
    const TransportationBase &base() const { return reinterpret_cast<const TransportationBase&>(*this); }

private:
    TransportationDerived<Car>   car_;
    TransportationDerived<Plane> plane_;
};

#endif
