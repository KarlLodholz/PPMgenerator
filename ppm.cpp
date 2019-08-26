#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
//#include "timer.h"
//using namespace std;

// compile with
// c++ ppm.cpp -o ppm `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

static GtkWidget *imgName_wdgt,*width_wdgt,*height_wdgt,*intensity_wdgt,*rLow_wdgt,*rHigh_wdgt,*gLow_wdgt,*gHigh_wdgt,*bLow_wdgt,*bHigh_wdgt,*x_start,*y_start,*color_button,*bias_wdgt,*error_output;

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

void nxtColor(Pixel **p,std::vector<int> &nxt,const int &x,const int &y,const int &width,int &red,int &green,int &blue, int &cntr){
    if(p[x][y].r == -1){
        if(p[x][y].g == -1) {  //if it has not been added to nxt
            nxt.push_back(posToNum(x,y,width));
            p[x][y].g = -2; //-2 means it has been added to nxt
        }
    }
    else {
        red += p[x][y].r;
        green += p[x][y].g;
        blue += p[x][y].b;
        cntr++;
    }
}

void locToPix(Pixel **p,std::vector<int> &nxt,const int &x,const int &y,const int &width,const int &height,
    int &red,int &green, int &blue,const int intensity, 
    const int &rRng,const int &gRng,const int &bRng,const int &rOfst, const int &gOfst, const int &bOfst) {
    
    bool left, up, down, right;
    int colorCntr = 0;
    
    //bool determining if the current pixel is within bounds
    left = x-1>=0;
    right = x+1<width;
    up = y-1>=0;
    down = y+1<height;
    if(up && left)
        nxtColor(p,nxt,x-1,y-1,width,red,green,blue,colorCntr);  
    if(up)
        nxtColor(p,nxt,x,y-1,width,red,green,blue,colorCntr);
    if(up & right)
        nxtColor(p,nxt,x+1,y-1,width,red,green,blue,colorCntr);   
    if(left)
        nxtColor(p,nxt,x-1,y,width,red,green,blue,colorCntr);  
    if(right)
        nxtColor(p,nxt,x+1,y,width,red,green,blue,colorCntr);  
    if(down & left)
        nxtColor(p,nxt,x-1,y+1,width,red,green,blue,colorCntr);  
    if(down)
        nxtColor(p,nxt,x,y+1,width,red,green,blue,colorCntr);  
    if(down && right)
        nxtColor(p,nxt,x+1,y+1,width,red,green,blue,colorCntr);  
    //get color avgs around pixel
    if(colorCntr) {
        red = (int)(red/(float)colorCntr+0.5) + (rand()%rRng+rOfst); //adding 0.5 creates a rounding effect to help balance integer devision
        green = (int)(green/(float)colorCntr+0.5) + (rand()%gRng+gOfst);
        blue = (int)(blue/(float)colorCntr+0.5) + (rand()%bRng+bOfst);
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

    p[x][y].setColor(red,green,blue);
    red = green = blue = 0;
    return;
}

void generate(GtkWidget *genBn, gpointer data) {
    
    int intensity = atoi((char *)gtk_entry_get_text(GTK_ENTRY(intensity_wdgt)));
    int rLow = atoi((char *)gtk_entry_get_text(GTK_ENTRY(rLow_wdgt)));
    int rHigh = atoi((char *)gtk_entry_get_text(GTK_ENTRY(rHigh_wdgt)))-rLow+1;
    int gLow = atoi((char *)gtk_entry_get_text(GTK_ENTRY(gLow_wdgt)));
    int gHigh = atoi((char *)gtk_entry_get_text(GTK_ENTRY(gHigh_wdgt)))-gLow+1;
    int bLow = atoi((char *)gtk_entry_get_text(GTK_ENTRY(bLow_wdgt)));
    int bHigh = atoi((char *)gtk_entry_get_text(GTK_ENTRY(bHigh_wdgt)))-bLow+1;
    int x = atoi((char *)gtk_entry_get_text(GTK_ENTRY(x_start)));
    int y = atoi((char *)gtk_entry_get_text(GTK_ENTRY(y_start)));
    std::string imgName = gtk_entry_get_text(GTK_ENTRY(imgName_wdgt));
    int width = atoi((char *)gtk_entry_get_text(GTK_ENTRY(width_wdgt)));
    int height = atoi((char *)gtk_entry_get_text(GTK_ENTRY(height_wdgt)));
    int bias = atoi((char *)gtk_entry_get_text(GTK_ENTRY(bias_wdgt)));
    bool valid = true;

    //std::cout<<"rLow:"<<rLow<<" rHigh:"<<rHigh<<std::endl;
    
    //converts the color button's color to 3 ints red, green, and blue;
    GdkRGBA *color = new GdkRGBA();
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(color_button), color);
    std::string temp_color = gdk_rgba_to_string(color);
    int red = (int)(stoi(temp_color.substr(temp_color.find_first_of('(')+1,temp_color.find_first_of(',')-temp_color.find_first_of('(')-1)) / 255.0 * intensity);
    temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
    int green = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(','))) / 255.0 * intensity);
    temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
    int blue = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(')'))) / 255.0 * intensity);

    // std::cout<<"("<<red<<","<<green<<","<<blue<<")"<<std::endl;
    // std::cout<<gdk_rgba_to_string(color)<<std::endl;
    gtk_label_set_text(GTK_LABEL(error_output),"");
    if(imgName == "") { 
        valid = false;
        gtk_label_set_text(GTK_LABEL(error_output),"Missing name of file");
    }
    else {
        if(width <= 0 || height <= 0){
            valid = false;
            gtk_label_set_text(GTK_LABEL(error_output),"Invalid Dimensions");
        }
        else {
            if(x >= width || x < -1 || y < -1 || y >= height) {
                valid = false;
                gtk_label_set_text(GTK_LABEL(error_output),"Invalid Starting position");
            }
        }
    }

    if(valid) {
        //Timer t;
        Pixel **p = new Pixel*[width];
        for(int i=0;i<width;i++){
            p[i] = new Pixel[height];
        }
        
        // its one big array of integers because poping is O(n);
        std::vector<int> nxt; 

        //startPix(p,nxt,width,height,intensity,rRng,gRng,bRng,rOfst,gOfst,bOfst); //get user input on starting pixels and starts pixels;

        std::ofstream ppm(imgName+".ppm");
        ppm<<"P3 "<<width<<" "<<height<<" "<<intensity<<"\n";

        //random starting position
        bool last = false;
        if(x == -1) {
            x = rand()%width;
            y = rand()%height;
        }
        locToPix(p,nxt,x,y,width,height,red,green,blue,intensity,rHigh,gHigh,bHigh,rLow,gLow,bLow);
        do {
            if(nxt.empty()) //only happen when nxt has no more pixels that need to be colored.
                last = true;
            if(!nxt.empty()) {
                int rando = rand()%nxt.size();
                for(int i=0;i<bias;i++)
                    if(rando)
                        rando = rand()%rando;
                numToPos(nxt[rando],x,y,width); //updates x and y to nxt[rando]
                std::swap(nxt[rando],nxt[nxt.size()-1]);
                nxt.pop_back();
            }
            locToPix(p,nxt,x,y,width,height,red,green,blue,intensity,rHigh,gHigh,bHigh,rLow,gLow,bLow);
        } while(!last);

        //printing arr p to ppm
        for(int posY=0;posY<height;posY++) {
            for (int posX=0;posX<width;posX++){
                ppm <<p[posX][posY].r<<" "
                    <<p[posX][posY].g<<" "
                    <<p[posX][posY].b<<" \t";
            }
            ppm<<"\n";
        }
        gtk_label_set_text(GTK_LABEL(error_output),(imgName+" has been successfully generated").c_str());
    }
    return;
}

int main(int argc, char **argv) {
    srand(time(NULL)); //init rand seed;
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *imgName_wdgt_label = gtk_label_new("Image Name:");
    imgName_wdgt = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt, 1, 0, 1, 1);

    GtkWidget *space0 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space0, 0, 1, 1, 1);

    GtkWidget *demensions_label = gtk_label_new("Dimensions  (Width,Height): ");
    width_wdgt = gtk_entry_new();
    height_wdgt = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), demensions_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_wdgt, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_wdgt, 2, 2, 1, 1);

    GtkWidget *space1 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space1, 0, 3, 1, 1);

    GtkWidget *intensity_wdgt_directions_pt0 = gtk_label_new("Higher number for more precision");
    GtkWidget *intensity_wdgt_directions_pt1 = gtk_label_new(" (must be less than 65536)");
    GtkWidget *intensity_wdgt_label = gtk_label_new("Color intensity: ");
    intensity_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_directions_pt0, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_directions_pt1, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt, 1, 5, 1, 1);
    
    GtkWidget *space2 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space2, 0, 6, 1, 1);

    GtkWidget *rng_label = gtk_label_new("Range of fluctuation");
    GtkWidget *rng_label_format = gtk_label_new("(minimum, maximum)");
    gtk_grid_attach(GTK_GRID(grid), rng_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rng_label_format, 1, 7, 1, 1);

    GtkWidget *rRng_label = gtk_label_new("Red Range: ");
    rLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    rHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), rRng_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rLow_wdgt, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rHigh_wdgt, 2, 8, 1, 1);

    GtkWidget *gRng_label = gtk_label_new("Green Range: ");
    gLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    gHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), gRng_label, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gLow_wdgt, 1, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gHigh_wdgt, 2, 9, 1, 1);

    GtkWidget *bRng_label = gtk_label_new("Blue Range: ");
    bLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    bHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), bRng_label, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bLow_wdgt, 1, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bHigh_wdgt, 2, 10, 1, 1);


    GtkWidget *space3 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space3, 0, 11, 1, 1);


    GtkWidget *starting_pnt_label_pt0 = gtk_label_new("Starting location");
    GtkWidget *starting_pnt_label_pt1 = gtk_label_new("-1 for random location");
    GtkWidget *starting_pnt_label = gtk_label_new("(x, y): ");
    x_start = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-1",2));
    y_start = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-1",2));
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label_pt0, 0, 12, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label_pt1, 1, 12, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label, 0, 13, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_start, 1, 13, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_start, 2, 13, 1, 1);

    GtkWidget *space4 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space3, 0, 14, 1, 1);

    GtkWidget *color_label = gtk_label_new("Starting color: ");
    bias_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    color_button = gtk_color_button_new();
    gtk_grid_attach(GTK_GRID(grid), color_label, 0, 15, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_button, 1, 15, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bias_wdgt, 2, 15, 1, 1);

    GtkWidget *genBn = gtk_button_new_with_label("Generate");
    g_signal_connect(genBn, "clicked", G_CALLBACK(generate), NULL);
    gtk_grid_attach(GTK_GRID(grid), genBn, 0, 16, 1, 1);
    
    error_output = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), error_output, 0, 17, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}