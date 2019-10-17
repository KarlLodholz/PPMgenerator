#ifndef PIXEL_H
#define PIXEL_H

class Pixel {
    public:
        int r,g,b; //should be a number between 0-intennsity
        Pixel(){r=-1;g=-1;b=-1;};
        void setColor(const int &red,const int &green,const int &blue) {r=red;g=green;b=blue;};
        
};

#endif