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
    int intensity;
    int rLow,rHigh,gLow,gHigh,bLow,bHigh;
    int width,height;
    int radius,bias;
    int red,green,blue;
    int pos = 0;
    int colorcntr = 0;
    Pixel *p;
    std::vector<int> nxt; 

    PPM(const std::string &name, const int &intensity, const int &width, const int &height, const int &pos,
        const int &rLow, const int &rHigh, const int &gLow, const int &gHigh, const int &bLow, const int &bHigh,
        const int &radius, const int &bias, const int &red, const int &green, const int &blue);

    ~PPM(){delete[] p;};

    void locToPix();
private:
    void nxtColor(const int &position);
};

PPM::PPM(const std::string &name, const int &intensity, const int &width, const int &height, const int &pos,
        const int &rLow, const int &rHigh, const int &gLow, const int &gHigh, const int &bLow, const int &bHigh,
        const int &radius, const int &bias, const int &red, const int &green, const int &blue) {
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
    this->gHigh = bHigh - bLow + 1;
    this->radius = radius;
    this->bias = bias;
    this->red = red / 255.0 * intensity;  //this converts the initial rgb to an rgb the a new intensity.
    this->green = green / 255.0 * intensity;
    this->blue = blue / 255.0 * intensity;
    p = new Pixel[width*height];
}

void PPM::locToPix() {
    
    bool left, up, down, right;
    colorcntr = 0;

    //bool determining if the current pixel is within bounds
    left = pos%width-1>=0;
    right = pos%width+1<width;
    up = pos/width-1>=0;
    down = pos/width+1<height;

    if(up && left)
        nxtColor(pos-width-1);  
    if(up)
        nxtColor(pos-width);
    if(up & right)
        nxtColor(pos-width+1);   
    if(left)
        nxtColor(pos-1);  
    if(right)
        nxtColor(pos+1);  
    if(down & left)
        nxtColor(pos+width-1);  
    if(down)
        nxtColor(pos+width);  
    if(down && right)
        nxtColor(pos+width+1);  
    
    //get color avgs around pixel
    if(colorcntr){
        red = (red/colorcntr) + (rand()%rHigh+rLow);
        green = (green/colorcntr) + (rand()%gHigh+gLow);
        blue = (blue/colorcntr) + (rand()%bHigh+bLow);
    }
    if(red > intensity)
        red = intensity;
    if(red < 0)
        red = 0;
    if(green > intensity)
        green = intensity;
    if(green < 0)
        green = 0;
    if(blue > intensity)
        blue = intensity;
    if(blue < 0)
        blue = 0;

    p[pos].setColor(red,green,blue);
    red = green = blue = 0;
    return;
}

void PPM::nxtColor(const int &position){
    if(p[position].r == -1){ //the color is -1 if the color has not been initialized for that individual pixel
        if(p[position].g == -1) {  //if it has been added to nxt
            nxt.push_back(position);
            p[position].g = -2; ////flag to check if it has been added to nxt
        }
    }
    else {
        red += p[position].r;
        green += p[position].g;
        blue += p[position].b;
        colorcntr++;
    }
    return;
}

#endif