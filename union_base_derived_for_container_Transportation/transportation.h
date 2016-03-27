#ifndef TRANSPORTATION_H
#define TRANSPORTATION_H

enum class TransEnum { car, plane};

class TransportationBase {
public:
    TransportationBase() {         std::cout << "\t\t\t\tCtor TransportationBase" << std::endl;
    }
    virtual ~TransportationBase() {std::cout << "\t\t\t\tDtor TransportationBase" << std::endl;
    }

    virtual unsigned speed() const =0;
    virtual void set_speed(unsigned speed) =0;

    virtual const char *name() const =0;
    virtual void set_name(const char *name) =0;

    virtual void move_forward() const =0;

    virtual TransEnum id() const =0;
};

template<typename Deriv>
class TransportationDerived : public TransportationBase, protected Deriv {
public:
    TransportationDerived(const char *name = "a transport mech.", unsigned speed = 0) : Deriv{name, speed} {}
    TransportationDerived(const TransportationDerived &rhs) : Deriv{rhs} {}
    TransportationDerived(const Deriv &rhs)                 : Deriv{rhs} {}

    virtual ~TransportationDerived() {}

    unsigned speed() const         override { return Deriv::speed(); }
    void set_speed(unsigned speed) override { return Deriv::set_speed(speed); }

    const char *name() const        override { return Deriv::name(); }
    void set_name(const char *name) override { return Deriv::set_name(name); }

    void move_forward() const override { return Deriv::move_forward(); }

    TransEnum id() const override { return Deriv::id(); }
};

#endif
