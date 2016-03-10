#ifndef RAY_H
#define _RAY_H

#include "math.h"

class Ray {
    Vect origin, direction;
    
public:
    
    Ray ();
    
    Ray (Vect, Vect);
    
    // method functions
    Vect getRayOrigin () { return origin; }
    Vect getRayDirection() { return direction; }    
    
};

Ray::Ray () {
    Vect a;
    Ray(a, a);
}

Ray::Ray (Vect origin, Vect direction) {
    this->origin = origin;
    this->direction = direction;
}

#endif