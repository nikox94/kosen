#ifndef Camera_H
#define _Camera_H

#include "math.h"

class Camera {
    Vect campos, camdir, camright, camdown;
    
public:
    
    Camera ();
    
    Camera (Vect, Vect, Vect, Vect);
    
    // method functions
    Vect getCameraPosition () { return campos; }
    Vect getCameraDirection() { return camdir; }   
    Vect getCameraRight() { return camright; }
    Vect getCameraDown() { return camdown; }
    
};

Camera::Camera () {
    Vect a;
    Vect Z = Vect(0,0,1);
    Camera(a, Z, a, a);
}

Camera::Camera (Vect campos, Vect camdir, Vect camright, Vect camdown) {
    this -> campos = campos;
    this -> camdir = camdir;
    this -> camdown = camdown;
    this -> camright = camright;
}

#endif