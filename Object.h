#ifndef OBJECT_H
#define OBJECT_H

#include <Ray.h>
#include <Vect.h>
#include <Color.h>

class Object {
        public:
        
        Object ();
        
        // method functions
        Color getObjectColor () { return Color(0.0, 0.0, 0.0, 0); }
        
        double findIntersection(Ray ray) {
            return 0;
        }
        
};

Object::Object () {}

#endif
