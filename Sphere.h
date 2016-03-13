#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include <Object.h>
#include <Vect.h>
#include <Color.h>

class Sphere : public Object {
        Vect center;
        double radius;
        Color color;
        
        public:
        
        Sphere ();
        
        Sphere (Vect, double, Color);
        
        // method functions
        Vect getCenter () { return center; }
        double getRadius() { return radius; }
        Color getColor () { return color; }
        
};

Sphere::Sphere () {
        center = Vect(0,0,0);
        radius = 1.0;
        color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere (Vect center, double radius, Color color) {
        this->center = center;
        this->radius = radius;
        this->color = color;
}

#endif
