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

#include <Object.h>
#include <Source.h>
#include <Vect.h>
#include <Ray.h>
#include <Camera.h>
#include <Color.h>
#include <Light.h>
#include <Sphere.h>
#include <Plane.h>
#include <Triangle.h>

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

int winningObjectIndex(vector<double> object_intersections) {
    // return the index of the winning intersections
    int index_of_minimum_value;

    // prevent unnessecary calculations
    if (object_intersections.size() == 0) {
        // if there are no intersections
        return -1;
    }

    if (object_intersections.size() == 1) {
        if (object_intersections.at(0) > 0) {
            // if that intersection is greater than zero then it's our index
            return 0;
        }
        else {
            // otherwise only intersection value is negative
            return -1;
        }
    }

    // otherwise there is more than one intersection
    // first find the maximum value
    double max = 0;
    for (int i = 0; i < object_intersections.size(); i++) {
        if (max < object_intersections.at(i)) {
            max = object_intersections.at(i);
        }
    }

    // then starting from the maximum value find the minimum possible index
    if (max > 0) {
        // we only want positive intersections
        // TODO: Algo can be optimised
        for (int index = 0; index < object_intersections.size(); index++) {
            if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                max = object_intersections.at(index);
                index_of_minimum_value = index;
            }
        }
        return index_of_minimum_value;
    }
    else {
        // all the intersections were negative
        return -1;
    }
}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects,
                 int index_of_winning_object, double accuracy, double ambientlight, vector<Source*> light_sources){
    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

    // Checkerboard pattern
    if (winning_object_color.getSpecial() == 2.0) {
        int square = (int) floor(intersection_position.getVectX()) + (int) floor(intersection_position.getVectZ());

        if ( square % 2 == 0 ) {
            // black tile
            winning_object_color.setRed(0);
            winning_object_color.setGreen(0);
            winning_object_color.setBlue(0);
        }
        else {
            //white tile
            winning_object_color.setRed(1);
            winning_object_color.setGreen(1);
            winning_object_color.setBlue(1);
        }
    }

    Color final_color = winning_object_color.scale(ambientlight);

    // Reflections
    if ( winning_object_color.getSpecial() > 0 && winning_object_color.getSpecial() <= 1) {
        // reflection from object with specular intensity
        double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
        Vect scaled1 = winning_object_normal.mult(dot1);
        Vect add1 = scaled1.add(intersecting_ray_direction);
        Vect scaled2 = add1.mult(2);
        Vect add2 = intersecting_ray_direction.negative().add(scaled2);
        Vect reflection_direction = add2.normalise();

        // create reflection ray
        Ray reflection_ray (intersection_position, reflection_direction);

        // determine what the ray intersects with first
        vector<double> reflection_intersections;

        // TODO: Think about a more natural limit for reflections
        for (int reflection_index = 0; reflection_index < scene_objects.size() ; reflection_index++) {
            reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
        }

        int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

        if (index_of_winning_object_with_reflection != -1) {
            // reflection ray did not intersect with anything else
            if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
                // determine position and direction at the point of intersection
                // with the ray
                // the ray only affects the color if it reflected off of something
                Vect reflection_intersection_position = intersection_position
                                                          .add(reflection_direction
                                                          .mult(reflection_intersections.at(index_of_winning_object_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;

                // Recursive call
                Color reflection_intersection_color = getColorAt(reflection_intersection_position,
                                                                 reflection_intersection_ray_direction,
                                                                 scene_objects,
                                                                 index_of_winning_object_with_reflection,
                                                                 accuracy,
                                                                 ambientlight,
                                                                 light_sources
                                                                );
                final_color = final_color.add(reflection_intersection_color.scale(winning_object_color.getSpecial()));
            }
        }
    }



    for (int light_index = 0; light_index < light_sources.size() ; light_index++) {
        Vect light_direction = light_sources.at(light_index)->getPosition().add(intersection_position.negative()).normalise();

        float cosine_angle = winning_object_normal.dot(light_direction);

        if (cosine_angle > 0) {
            // test for shadows
            bool shadowed = false;

            Vect distance_to_light = light_sources.at(light_index)->getPosition().add(intersection_position.negative()).normalise();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray (intersection_position, light_sources.at(light_index)->getPosition().add(intersection_position.negative()).normalise());

            vector<double> secondary_intersections;

            for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
                secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            }

            for (int c = 0; c < secondary_intersections.size(); c++) {
                if (secondary_intersections.at(c) > accuracy) {
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                    break;
                }
            }

            if (shadowed == false) {
                final_color = final_color.add(winning_object_color.multiply(light_sources.at(light_index)->getColor().scale(cosine_angle)));

                if (winning_object_color.getSpecial() > 0 && winning_object_color.getSpecial() <= 1) {
                    // special \in [0,1]
                    double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.mult(dot1);
                    Vect add1 = scalar1.add(intersecting_ray_direction);
                    Vect scalar2 = add1.mult(2);
                    Vect add2 = intersecting_ray_direction.negative().add(scalar2);
                    Vect reflection_direction = add2.normalise();

                    double specular = reflection_direction.dot(light_direction);
                    if (specular > 0) {
                        specular = pow(specular, 10);
                        final_color = final_color.add(light_sources.at(light_index)->getColor().scale(specular*winning_object_color.getSpecial()));
                    }
                }
            }
        }
    }

    return final_color.clip();
}

int main (int argc, char *argv[]) {
    cout << "rendering..." << endl;

    // Measure the time of execution of the rendering
    clock_t t1, t2;
    t1 = clock();

    int dpi = 72;
    int width = 640;
    int height = 480;
    int n = width*height;
    RGBType *pixels = new RGBType[n];

    int aadepth = 1; // Anti-aliasing depth
    double aathreshold = 0.1;
    double aspectratio = (double) width / (double) height;
    double ambientlight = 0.2;
    double accuracy = 0.000001;

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
    Color yellow (1.0, 1.0, 0.0, 2);
    Color red (1, 0, 0, 0.5);
    Color cyan (0, 1, 1, 0.2);
    Color black (0.0, 0.0, 0.0, 0.0);

    Vect light_position (-7, 10, -10);
    Light scene_light (light_position, white_light);
    vector<Source*> light_sources;
    light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //scene objects
    Sphere scene_sphere (O, 1, green);
    Sphere scene_sphere_2 (Z.mult(5), 1, red);
    Plane scene_plane (Y, -1, yellow);
//     Triangle scene_triangle(Vect(3,0,0),
//                             Vect(0,3,0),
//                             Vect(0,0,3),
//                             cyan);

    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere_2));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
//     scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));


    int thisone, aa_index;
    double xamnt, yamnt;
    // Anti-aliasing
    // Start with a blank pixel
    double tempRed[aadepth*aadepth];
    double tempGreen[aadepth*aadepth];
    double tempBlue[aadepth*aadepth];

    // pixel main loop
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            thisone = y*width + x;

            // Anti-aliasing loop
            for (int aax = 0; aax < aadepth; aax++) {
                for (int aay = 0; aay < aadepth; aay++) {
                    aa_index = aay*aadepth + aax;
                    srand(time(0));

                    // create the ray from the camera to this pixel
                    if (aadepth == 1) {
                        // No anti-aliasing
                        if (width > height) {
                            // the image is wider than it is tall
                            xamnt = ((x+0.5)/width)*aspectratio - ((width-height)/ (double) height);
                            yamnt = ((height - y) + 0.5)/height;
                        }
                        else if (height > width) {
                            // the image is taller than it is wide
                            xamnt = (x+0.5)/width;
                            yamnt = (((height - y) + 0.5)/height)/aspectratio - ((height - width)/(double) width)/2;
                        }
                        else {
                            // the image is square
                            xamnt = (x+0.5)/width;
                            yamnt = ((height - y) + 0.5)/height;
                        }
                    }
                    else
                    {
                        double offset = (double) aax/((double) aadepth - 1);
                        // anti-aliasing
                        if (width > height) {
                            // the image is wider than it is tall
                            xamnt = (((x + offset)/width)*aspectratio - ((width-height)/ (double) height));
                            yamnt = ((height - y) + offset)/height;
                        }
                        else if (height > width) {
                            // the image is taller than it is wide
                            xamnt = (x+offset)/width;
                            yamnt = (((height - y) + offset)/height)/aspectratio - ((height - width)/(double) width)/2;
                        }
                        else {
                            // the image is square
                            xamnt = (x+offset)/width;
                            yamnt = ((height - y) + offset)/height;
                        }
                    }

                    Vect cam_ray_origin = scene_cam.getCameraPosition();
                    Vect cam_ray_direction = camdir.add(camright.mult(xamnt-0.5).add(camdown.mult(yamnt - 0.5))).normalise();

                    Ray cam_ray (cam_ray_origin, cam_ray_direction);

                    // Calculate all object intersections with cam_ray
                    vector<double> intersections;

                    for (int index = 0; index < scene_objects.size(); index++) {
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
                    }

                    int index_of_winning_object = winningObjectIndex(intersections);

                    if(index_of_winning_object == -1) {
                        tempRed[aa_index] = 0.0;
                        tempGreen[aa_index] = 0.0;
                        tempBlue[aa_index] = 0.0;
                    }
                    else {
                        if(intersections.at(index_of_winning_object) > accuracy) {
                            // determine the position and direction vectors at intersection

                            Vect intersection_position = cam_ray_origin.add(cam_ray_direction.mult(intersections.at(index_of_winning_object)));
                            Vect intersecting_ray_direction = cam_ray_direction;

                            Color current_obj_color = getColorAt(intersection_position,
                                                                 intersecting_ray_direction,
                                                                 scene_objects,
                                                                 index_of_winning_object,
                                                                 accuracy,
                                                                 ambientlight,
                                                                 light_sources);
                            tempRed[aa_index] = current_obj_color.getRed();
                            tempGreen[aa_index] = current_obj_color.getGreen();
                            tempBlue[aa_index] = current_obj_color.getBlue();
                        }
                    }
                    // end of Anti-aliasing loop
                }
            }
            // end of pixel loop

            // average the pixel color
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;

            for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
                totalRed += tempRed[iRed];
            }

            for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
                totalGreen += tempGreen[iGreen];
            }

            for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
                totalBlue += tempBlue[iBlue];
            }

            double avgRed = totalRed/(aadepth*aadepth);
            double avgGreen = totalGreen/(aadepth*aadepth);
            double avgBlue = totalBlue/(aadepth*aadepth);

            pixels[thisone].r = avgRed;
            pixels[thisone].g = avgGreen;
            pixels[thisone].b = avgBlue;
        }
    }
    savebmp("scene.bmp", width, height, dpi, pixels);

    delete pixels, tempRed, tempGreen, tempBlue;

    // Calculate the time for rendering
    t2 = clock();
    float diff = ( (float) t2 - (float) t1) / CLOCKS_PER_SEC;
    cout << "This rendering took "<< diff << "seconds.\n";

    return 0;
}
