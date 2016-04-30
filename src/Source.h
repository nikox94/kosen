#ifndef Source_H
#define Source_H

#include <Vect.h>
#include <Color.h>

/**
 * Virtual class for any type of light source.
 */
class Source {
public:

    Source ();

    // method functions
    virtual Vect getPosition() {return Vect(0, 0, 0);}

    virtual Color getColor () { return Color(1, 1, 1, 0); }

};

Source::Source () {}

#endif
