#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <Vect.h>
#include <Ray.h>
#include <Camera.h>
#include <Color.h>
#include <Light.h>
#include <Sphere.h>
#include <Plane.h>

using namespace std;

struct RGBType {
    double r;
    double g;
    double b;
};

void savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0,  0, 0, 0, 0,  54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[ 10] = (unsigned char)(h>>16);
    bmpinfoheader[ 11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for (int i = 0; i < k; i++) {
        RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

        fwrite(color,1,3,f);
    }

    fclose(f);
}

int thisone;

int main (int argc, char *argv[]) {
    cout << "rendering..." << endl;

    int dpi = 72;
    int width = 640;
    int height = 480;
    int n = width*height;
    RGBType *pixels = new RGBType[n];

    Vect O (0,0,0);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

    Vect campos (3, 1.5, -4);

    Vect look_at (0, 0, 0);
    Vect diff_btw = campos.add(look_at.negative());

    Vect camdir = diff_btw.negative().normalise();
    Vect camright = Y.cross(camdir).normalise();
    Vect camdown = camright.cross(camdir);
    Camera scene_cam (campos, camdir, camright, camdown);

    Color white_light (1.0, 1.0, 1.0, 0.0);
    Color green (0.5, 1.0, 0.5, 0.3);
    Color cyan (1.0, 1.0, 0.0, 0.0);
    Color gray (0.5, 0.5, 0.5, 0.0);
    Color black (0.0, 0.0, 0.0, 0.0);

    Vect light_position (-7, 10, -10);
    Light scene_light (light_position, white_light);

    //scene objects
    Sphere scene_sphere (O, 1, green);
    Plane scene_plane (Y, -1, cyan);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            thisone = j*width + i;

            pixels[thisone].r = 0.1+0.001*i+0.1*j;
            pixels[thisone].g = 0.8+0.001*i-0.1*j;
            pixels[thisone].b = 0.05+0.001*i+0.1*j;
        }
    }
    savebmp("scene.bmp", width, height, dpi, pixels);

    return 0;
}
