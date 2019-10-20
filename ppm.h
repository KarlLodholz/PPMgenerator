#ifndef PPM_H
#define PPM_H

#include "pixel.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio> 
#include <stdlib.h>
#include <stdio.h>

class PPM {
public:
    std::string name;
    int width,height;
    int intensity;
    int bias;
    int pos = 0;
    Pixel *p;
    std::vector<int> nxt; 

    PPM(const std::string &name, const int &intensity, const int &width, const int &height, const int &pos,
        const int &rLow, const int &rHigh, const int &gLow, const int &gHigh, const int &bLow, const int &bHigh,
        const int &color_radius, const int &growth_radius, const int &bias, const int &red, const int &green, const int &blue);

    ~PPM(){delete[] p;};

    void get_neighbor_color();
private:
    int rLow,rHigh,gLow,gHigh,bLow,bHigh;
    int growth_radius; //tiles within this radius up, down, left, and right will be added to the nxt array
    int color_radius; //how many tiles out up, down, left, and right will be observed to avg color.
    int init_red,init_green,init_blue,red,green,blue;
    int colorcntr = 0;
    void add_color(const int &position);
    void add_neighbor(const int &position);
};

PPM::PPM(const std::string &name, const int &intensity, const int &width, const int &height, const int &pos,
        const int &rLow, const int &rHigh, const int &gLow, const int &gHigh, const int &bLow, const int &bHigh,
        const int &color_radius, const int &growth_radius, const int &bias, const int &red, const int &green, const int &blue) {
    this->name = name;
    this->intensity = intensity;
    this->width = width;
    this->height = height;
    this->pos = pos;
    this->rLow = rLow;
    this->rHigh = rHigh - rLow + 1;
    this->gLow = gLow;
    this->gHigh = gHigh - gLow + 1;
    this->bLow = bLow;
    this->bHigh = bHigh - bLow + 1;
    this->color_radius = color_radius; 
    this->growth_radius = growth_radius; 
    this->bias = bias;
    this->init_red = red / 255.0 * intensity;  //this converts the initial rgb to an rgb the a new intensity.
    this->init_green =  green / 255.0 * intensity;
    this->init_blue = blue / 255.0 * intensity;
    p = new Pixel[width*height];
}

//gets the mean avg of the tiles' colors within the radius of color_radius.
//the init color will be choosen if there is no neighboring tiles within the growth radius to have color collected from
void PPM::get_neighbor_color() {
    colorcntr = 0;
    int left_radius, up_radius, down_radius, right_radius;
    left_radius = up_radius = down_radius = right_radius = (color_radius>=growth_radius?color_radius:growth_radius);
    //std::cout<<left_radius<<"  "<<color_radius<<"  "<<growth_radius<<std::endl;
    red = green = blue = 0;
    //reduces the radii down within bounds of the picture if needed
    while(pos%width-left_radius<0)left_radius--;
    while(pos%width+right_radius>=width)right_radius--;
    while(pos/width-up_radius<0)up_radius--;
    while(pos/width+down_radius>=height)down_radius--;

    //get color avgs around pixel and add neighbors to nxt
    for(int y = 0-up_radius; y <= down_radius; y++) {
        for(int x = 0-left_radius; x<= right_radius; x++) {
            if(abs(x)<=growth_radius && abs(y)<=growth_radius) add_neighbor(pos+width*y+x);
            if(abs(x)<=color_radius && abs(y)<=color_radius) add_color(pos+width*y+x);
        }
    }
    
    if(colorcntr) {
        red = (red/colorcntr) + (rand()%rHigh+rLow);
        green = (green/colorcntr) + (rand()%gHigh+gLow);
        blue = (blue/colorcntr) + (rand()%bHigh+bLow);
    }
    else {
        red = init_red;
        green = init_green;
        blue = init_blue;

    }
    //keeps the values for color within the bounds of the color spectrum
    if(red > intensity) red = intensity;
    if(red < 0) red = 0;
    if(green > intensity) green = intensity;
    if(green < 0) green = 0;
    if(blue > intensity) blue = intensity;
    if(blue < 0) blue = 0;
    
    p[pos].setColor(red,green,blue);
    return;
}

void PPM::add_color(const int &position) {
    if(p[position].r >= 0){ //the color is -1 if the color has not been initialized for that individual pixel
        red += p[position].r;
        green += p[position].g;
        blue += p[position].b;
        colorcntr++;
    }
    return;
}

void PPM::add_neighbor(const int &position) {
    if(p[position].r == -1) {  //if it has been added to nxt
        nxt.push_back(position);
        p[position].r = -2; ////flag to check if it has been added to nxt
    }
    return;
}

#endif