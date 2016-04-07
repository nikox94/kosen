# kosen
A simple raytracer in C++

##Installation

First, download the source code of this repository to some convenient and happy folder using:

```bash
git clone <repository link (HTTPS preferred)>
```

Next, make sure you have a C++ compiler installed. I use `g++`.

You can use the `g++` compiler on Linux or [MinGW](http://www.mingw.org/) for Windows
(don't forget to add the `bin` directory of MinGW to the `PATH` environment variable, for easy access to `g++` from the cmd.)

Then you can simply run the makefile to build the project. In the command line type:
```bash
make
```

This should generate an executable called `kosen` that you can run with a scene file as an argument, e.g.:
```bash
kosen scene5.test
```
A bash file called `run.sh` should soon be written that will automatically run kosen
for all the sample scenefiles provided in the project subdirectory `sample scene files`.

##Scene file commands

The input file consists of a sequence of lines, each of which has a command. It is used to set up and describe a scene for rendering.
For examples and clarifications, see the example input files. The lines have the following form.

* **# comments** This is a line of comments. Ignore any line starting with a #.
* **Blank line** The input file can have blank lines that should be ignored.
* **command parameter1 parameter2 ...** The first part of the line is always the command. Based on what the command is, it has certain parameters which should be parsed appropriately.

###General

* `size width height`: The size command must be the first command of the file, which controls the image size.
* `maxdepth depth`: The maximum depth (number of bounces) for a ray (default should be 5).
* `output filename`: The output file to which the image should be written. You can either require this to be specified in the input, or you can use a suitable default like stdout or raytrace.bmp (The final files should be .bmp).

###Camera

The camera is specified as follows. In general, there should be only one camera specification in the input file;
what happens if there is more than one specification is left undefined.

* `camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz` specifies the camera in the standard way.

###Graphics

We only have spheres, triangles and planes so far.


* `sphere x y z radius` Defines a sphere with a given position and radius.
* `maxverts number` Defines a maximum number of vertices for later triangle specifications. It must be set before vertices are defined. (Your program may not need this; it is simply a convenience to allocate arrays accordingly)
* `vertex x y z` Defines a vertex at the given location. The vertex is put into a pile, starting to be numbered at 0.
* `tri v1 v2 v3` Create a triangle out of the vertices involved (which have previously been specified with the vertex command). The vertices are assumed to be specified in counter-clockwise order. Your code should internally compute a face normal for this triangle.
* `plane normalx normaly normalz distance` Defines a plane using the normal to the plane and its distance from the origin.

###Transformations

For triangles, we simply transform them to the eye coordinates and store them there.
For spheres, we store the transformation with them, doing the trick of pre-transforming the ray,
intersecting with a sphere, and then post-transforming the intersection point.
For planes, no transformations have been implemented yet.

* `translate x y z` A translation 3-vector.
* `rotate x y z angle` Rotate by angle (in degrees) about the given axis as in OpenGL.
* `scale x y z` Scale by the corresponding amount in each axis (a non-uniform scaling).
* `pushTransform` Push the current modeling transform on the stack as in OpenGL.
* `popTransform` Pop the current transform from the stack as in OpenGL.

###Lights

* `directional x y z r g b` The direction to the light source, and the color, as in OpenGL.
* `point x y z r g b` The location of a point source and the color, as in OpenGL.
* `attenuation const linear quadratic` Sets the constant, linear and quadratic attenuations (default 1,0,0) as in OpenGL. By default there is no attenuation (the constant term is 1, linear and quadratic are 0; that's what we mean by 1,0,0).
* `ambient r g b` The global ambient color to be added for each object (default is .2,.2,.2).

Note that we allow the ambient value to be changed between objects (so different objects will be rendered with a different ambient term;
this is used frequently in the example files).
Finally, note that here and in the materials below, we do not include the alpha term in the color specification.

###Materials


* `diffuse r g b` specifies the diffuse color of the surface.
* `specular r g b` specifies the specular color of the surface.
* `shininess s` specifies the shininess of the surface. _Note_: Putting the shininess to 2 will give a special checkered pattern to planes. Try it!
* `emission r g b` gives the emissive color of the surface.

#Contributing

This repository will be happy about any contributions, comments and pull requests.
