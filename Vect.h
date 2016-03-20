#ifndef VECT_H
#define VECT_H

#include "math.h"

class Vect {
    double x, y, z;

public:

    Vect ();

    Vect (double, double, double);

    // method functions
    double getVectX() { return x; }
    double getVectY() { return y; }
    double getVectZ() { return z; }

    double magnitude() {
        return sqrt(x*x+y*y+z*z);
    }

    Vect normalise() {
        double mag = magnitude();
        return Vect(x/mag, y/mag, z/mag);
    }

    Vect negative () {
        return Vect(-x,-y,-z);
    }

    double dot (Vect v) {
        return x*v.x + y*v.y + z*v.z;
    }

    Vect cross (Vect v) {
        return Vect(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

    Vect add (Vect v) {
        return Vect(x+v.x, y+v.y, z+v.z);
    }

    Vect mult (double s) {
        return Vect(s*x, s*y, s*z);
    }
};

Vect::Vect () {
    Vect(0, 0, 0);
}

Vect::Vect (double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

#endif
