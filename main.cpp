#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

class Pixel {
    public:
        int r,g,b; //should be a number between 0-intennsity
        Pixel(){r=-1;g=-1;b=-1;};
        void setColor(const int &red,const int &green,const int &blue) {r=red;g=green;b=blue;};
        
};

int posToNum(const int &x, const int &y, const int &width) {
    return (y*(width)+x);
}

void numToPos(const int &num, int &x, int &y, const int &width) {
    y = (int)(num / (width));
    x = num % (width);
    return;
}

void demensions(int &x, int &y) {
    int answ;
    cout<<"0:854x480 \n1:1024x576 \n2:1280x720 \n3:1920x1080 \n4:2560x1440 \n5:3840x2160 \n6:custom\nDimensions: ";
    cin>>answ;
    switch(answ){
        case 0:
            x = 854;
            y = 480;
            break;
        case 1:
            x = 1024;
            y = 576;
            break;
        case 2:
            x = 1280;
            y = 720;
            break;
        case 3:
            x = 1920;
            y = 1080;
            break;
        case 4:
            x = 2560;
            y = 1440;
            break;
        case 5:
            x = 3840;
            y = 2160;
            break;
        case 6:
            cout<<"Enter width: ";
            cin>>x;
            cout<<"Enter height: ";
            cin>>y;
            break;
    }
    return;
}

void gradiation(int &intensity, int &rRng,int &rOfst,int &gRng,int &gOfst,int &bRng,int &bOfst) {
    int rh,gh,bh;
    cout<<"Higher number for more precision (recommended at least 255)\nEnter color intensity:";
    cin>>intensity;
    cout<<"Enter a range of the possible fluctuation of color per pixel based on intensity\n";
    cout<<"Red min: ";
    cin>>rOfst;
    cout<<"Red max: ";
    cin>>rh;
    cout<<"Green min: ";
    cin>>gOfst;
    cout<<"Green max: ";
    cin>>gh;
    cout<<"Blue min: ";
    cin>>bOfst;
    cout<<"Blue max: ";
    cin>>bh;
    rRng = rh-rOfst+1;
    gRng = gh-gOfst+1;
    bRng = bh-bOfst+1;
    return;
}

void nxtColor(Pixel **p,vector<int> &nxt,const int &x,const int &y,const int &width,int &red,int &green,int &blue, int &cntr){
    if(p[x][y].r == -1){
        if(find(nxt.begin(),nxt.end(),posToNum(x,y,width)) == nxt.end())
            nxt.push_back(posToNum(x,y,width));
    }
    else {
        red += p[x][y].r;
        green += p[x][y].g;
        blue += p[x][y].b;
        cntr++;
    }
}

// void startPix(Pixel **p, vector<int> &nxt) {
//     int pixNum;
//     cout<<"Enter number of starting pixels: ";
//     cin>>pixNum;
//     pixNum 
// }

int main() {
    
    srand(time(NULL)); //init rand seed;

    string imgName;
    int width,height;
    int red,green,blue;
    int intensity,rRng,rOfst,gRng,gOfst,bRng,bOfst; //used for gradient control

    cout<<"Warning: Entering a name of an existing file will overwrite it!\n";
    cout<<"Enter the name of the img: ";
    cin>>imgName;
    demensions(width,height); //gets user input on demensions and sets them
    gradiation(intensity,rRng,rOfst,gRng,gOfst,bRng,bOfst); //gets user input on gradient

    Pixel **p = new Pixel*[width];
    for(int i=0;i<width;i++){
        p[i] = new Pixel[height];
    }
    vector<int> nxt;

    // startPix(p,nxt); //get user input on starting pixels and starts pixels;

    cout<<"Enter red(0-255):";
    cin>>red;
    red = (int)red/255*intensity; //converts red to correct color given changing intensity 
    cout<<"Enter green(0-255): ";
    cin>>green;
    green = (int)green/255*intensity; //converts green to correct color given changing intensity 
    cout<<"Enter blue(0-255): "; 
    cin>>blue;
    blue = (int)blue/255*intensity; //converts blue to correct color given changing intensity 

    ofstream ppm(imgName+".ppm");
    ppm<<"P3 "<<width <<" "<<height<<" "<<intensity<<"\n";

    //random starting position
    bool up,down,left,right,last = false;
    
    int posX;
    int posY;

    posX = rand()%width;
    posY = rand()%height;
    
    int colorCntr;

    do {
        colorCntr = 0;
        
        //bool determining if the current pixel is within bounds
        left = posX-1>=0;
        right = posX+1<width;
        up = posY-1>=0;
        down = posY+1<height;
        
        if(up && left)
            nxtColor(p,nxt,posX-1,posY-1,width,red,green,blue,colorCntr);  
        if(up)
            nxtColor(p,nxt,posX,posY-1,width,red,green,blue,colorCntr);
        if(up & right)
            nxtColor(p,nxt,posX+1,posY-1,width,red,green,blue,colorCntr);   
        if(left)
            nxtColor(p,nxt,posX-1,posY,width,red,green,blue,colorCntr);  
        if(right)
            nxtColor(p,nxt,posX+1,posY,width,red,green,blue,colorCntr);  
        if(down & left)
            nxtColor(p,nxt,posX-1,posY+1,width,red,green,blue,colorCntr);  
        if(down)
            nxtColor(p,nxt,posX,posY+1,width,red,green,blue,colorCntr);  
        if(down && right)
            nxtColor(p,nxt,posX+1,posY+1,width,red,green,blue,colorCntr);  
        
        //get color avgs around pixel
        if(colorCntr){
            red = (red/colorCntr) + (rand()%rRng+rOfst);
            green = (green/colorCntr) + (rand()%gRng+gOfst);
            blue = (blue/colorCntr) + (rand()%bRng+bOfst);
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

        if(nxt.empty())
            last = true;

        p[posX][posY].setColor(red,green,blue);

        // get next pixel
        // for(int i=0;i<nxt.size();i++)
        //     cout<<nxt[i]<<" ";
        // cout<<"mo"<<endl;
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