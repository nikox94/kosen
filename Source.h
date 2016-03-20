#ifndef Source_H
#define Source_H

#include <Ray.h>
#include <Vect.h>
#include <Color.h>

class Source {
        public:

        Source ();

        // method functions
        virtual Vect getPosition() {return Vect(0, 0, 0);}

        virtual Color getColor () { return Color(1, 1, 1, 0); }

};

Source::Source () {}

#endif
