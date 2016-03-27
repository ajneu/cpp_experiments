#include <iostream>
#include <array>
#include <algorithm>


#include "car.h"
#include "plane.h"
#include "transportation.h"
#include "transu.h"

int main()
{
    {
        Car car;
        std::cout << car.name() << std::endl;
        car.move_forward();
        std::cout << std::endl;
        
        Plane plane;
        std::cout << plane.name() << std::endl;
        plane.move_forward();
        std::cout << std::endl;

        std::cout << "sizeof(Car)   == " << sizeof(Car) << std::endl;
        std::cout << "sizeof(Plane) == " << sizeof(Plane) << std::endl;
        std::cout << "            (comment: not derived from any interface or anything, so minimal)\n" << std::endl;
    }

    std::cout << "-----------------------------------------" << std::endl;

    {
        TransportationDerived<Car>   car;
        TransportationDerived<Plane> plane;

        std::array<TransportationBase *, 2> arr{{&car, &plane}};

        std::for_each(arr.cbegin(), arr.cend(), [](const TransportationBase *v) {
                std::cout << v->name() << std::endl;
                v->move_forward();
                std::cout << std::endl;
            });

        std::cout << "sizeof(TransportationDerived<Car>)   == " << sizeof(TransportationDerived<Car>) << std::endl;
        std::cout << "sizeof(TransportationDerived<Plane>) == " << sizeof(TransportationDerived<Plane>) << std::endl;
        std::cout << "            (comment: derived an abstract base class, so minimally (sizeof(void*)) larger than above)\n" << std::endl;
    }
    
    std::cout << "-----------------------------------------" << std::endl;

    {
        TransU car{TransEnum::car, "a cool car", 100};
        TransU plane{TransEnum::plane, "asdf", 40};

        std::array<TransU, 2> arr{{{TransEnum::car, "a cool car", 100}, {TransEnum::plane, "a cool plane", 300}}};
        std::for_each(arr.cbegin(), arr.cend(), [](const TransU &v) {
                auto &b = v.base();
                std::cout << b.name() << std::endl;
                b.move_forward();
                std::cout << std::endl;
            });
    }

    
    return 0;
}
