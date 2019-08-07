#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

class Pixel {
    public:
        int r,g,b; //should be a number between 0-255
        Pixel(){r=-1;g=-1;b=-1;};
        void setColor(const int &red,const int &green,const int &blue) {r=red;g=green;b=blue;};
        
};

int posToNum(const int &x, const int &y, const int &width) {
    //cout<<"pos-num num:"<<(y*(width)+x)<<" x:"<<x<<" y:"<<y<<endl;
    return (y*(width)+x);
}

void numToPos(const int &num, int &x, int &y, const int &width) {
    y = (int)(num / (width));
    x = num % (width);
    //cout<<"num-pos y:"<<y<<" x:"<<x<<"num:"<<num<<endl;
    return;
}


// void nxtColor(Pixel p[][],vector<int nxt,const int &x,const int &y,const int &width,int &red,int &green,int &blue, int &cntr){
//     if(p[x][y].r == -1)
//         if(find(nxt.begin(),nxt.end(),posToNum(x,y,width)) == nxt.end())
//             nxt.push_back(posToNum(x,y-1,width));
//     else {
//         red += p[x][y].r;
//         green += p[x][y].g;
//         blue += p[x][y].b;
//         cntr++;
//     }
// }

// void avgColorSum(Pixel **p, const int &x, const int &y, int &red, int &green, int &blue, int &cntr) {

//     cout<<"red:"<<p[x][y].r<<" green:"<<green<<" blue:"<<blue<<" cntr:"<<cntr<<endl;
//     cntr++;
//     return;
// }

int main() {

    srand(time(NULL)); //init rand seed;

    string imgName;
    int width,height,red,green,blue,intensity=255;
    cout<<"Warning: Entering a name of an existing file will overwrite it!\n";
    cout<<"Enter the name of the img: ";
    cin>>imgName;
    cout<<"Enter width: ";
    cin>>width;
    cout<<"Enter height: ";
    cin>>height;
    cout<<"Enter red: ";
    cin>>red;
    cout<<"Enter green: ";
    cin>>green;
    cout<<"Enter blue: ";
    cin>>blue;

    ofstream ppm(imgName+".ppm");
    ppm<<"P3 "<<width <<" "<<height<<" "<<intensity<<"\n";

    Pixel **p = new Pixel*[width];
    for(int i=0;i<width;i++){
        p[i] = new Pixel[height];
    }
    //Pixel p[width][height];

    //random starting position
    bool up,down,left,right,last = false;
    int posX = rand()%width;
    int posY = rand()%height;
    int colorCntr;
    
    vector<int> nxt;
    do {
        colorCntr = 0;
        up = false;
        down = false;
        left = false;
        right = false;
        if(posX-1>=0)
            left = true;
        if(posX+1<width)
            right = true;
        if(posY-1>=0)
            up = true;
        if(posY+1<height)
            down = true;
        
        if(up && left)
            if(p[posX-1][posY-1].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX-1,posY-1,width)) == nxt.end())
                    nxt.push_back(posToNum(posX-1,posY-1,width));
            }
            else {
                red += p[posX-1][posY-1].r;
                green += p[posX-1][posY-1].g;
                blue += p[posX-1][posY-1].b;
                colorCntr++;
            }
        if(up)
            if(p[posX][posY-1].r == -1) {
                    if(find(nxt.begin(),nxt.end(),posToNum(posX,posY-1,width)) == nxt.end())
                        nxt.push_back(posToNum(posX,posY-1,width));
            }
            else {
                red += p[posX][posY-1].r;
                green += p[posX][posY-1].g;
                blue += p[posX][posY-1].b;
                colorCntr++;
            }
        if(up & right)
            if(p[posX+1][posY-1].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX+1,posY-1,width)) == nxt.end())
                    nxt.push_back(posToNum(posX+1,posY-1,width));
            }
            else {
                red += p[posX+1][posY-1].r;
                green += p[posX+1][posY-1].g;
                blue += p[posX+1][posY-1].b;
                colorCntr++;
            }
        if(left)
            if(p[posX-1][posY].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX-1,posY,width)) == nxt.end())
                    nxt.push_back(posToNum(posX-1,posY,width));
            }
            else {
                red += p[posX-1][posY].r;
                green += p[posX-1][posY].g;
                blue += p[posX-1][posY].b;
                colorCntr++;
            }
        if(right)
            if(p[posX+1][posY].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX+1,posY,width)) == nxt.end())
                    nxt.push_back(posToNum(posX+1,posY,width));
            }
            else {
                red += p[posX+1][posY].r;
                green += p[posX+1][posY].g;
                blue += p[posX+1][posY].b;
                colorCntr++;
            }
        if(down & left)
            if(p[posX-1][posY+1].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX-1,posY+1,width)) == nxt.end())
                    nxt.push_back(posToNum(posX-1,posY+1,width));
            }
            else {
                red += p[posX-1][posY+1].r;
                green += p[posX-1][posY+1].g;
                blue += p[posX-1][posY+1].b;
                colorCntr++;
            }
        if(down)
            if(p[posX][posY+1].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX,posY+1,width)) == nxt.end())
                    nxt.push_back(posToNum(posX,posY+1,width));
            }
            else {
                red += p[posX][posY+1].r;
                green += p[posX][posY+1].g;
                blue += p[posX][posY+1].b;
                colorCntr++;
            }
        if(down && right)
            if(p[posX+1][posY+1].r == -1) {
                if(find(nxt.begin(),nxt.end(),posToNum(posX+1,posY+1,width)) == nxt.end())
                    nxt.push_back(posToNum(posX+1,posY+1,width));
            }
            else {
                red += p[posX+1][posY+1].r;
                green += p[posX+1][posY+1].g;
                blue += p[posX+1][posY+1].b;
                colorCntr++;
            }
        //get color avgs around pixel
        if(colorCntr){
            red = (red/colorCntr) + (rand()%70-36);
            green = (green/colorCntr) + (rand()%3-1);
            blue = (blue/colorCntr) + (rand()%70-35);
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
        }
        // red = rand()%255;
        // green = rand()%255;
        // blue = rand()%255;

        if(nxt.empty())
            last = true;

        p[posX][posY].setColor(red,green,blue);

        //get next pixel
        // for(int i=0;i<nxt.size();i++)
        //     cout<<nxt[i]<<" ";
        // cout<<endl;
        if(!nxt.empty()) {
            int rando = rand()%nxt.size();
            // cout<<"rando:"<<rando<<" elem:"<<nxt[rando]<<endl;
            numToPos(nxt[rando],posX,posY,width); //posX and posY will now point to new position
            swap(nxt[rando],nxt[nxt.size()-1]);
            nxt.pop_back();
        }
        red = green = blue = 0;
    } while(!last);

    //printing arr p to ppm
    for(int y=0;y<height;y++) {
        for (int x=0;x<width;x++){
            ppm <<p[x][y].r<<" "
                <<p[x][y].g<<" "
                <<p[x][y].b<<" \t";
        }
        ppm<<"\n";
    }

    return 0;
}