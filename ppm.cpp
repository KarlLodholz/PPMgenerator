#include "ppm.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

//#include "timer.h"

// compile with
// c++ ppm.cpp -o ppm `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

static GtkWidget *imgName_wdgt,*width_wdgt,*height_wdgt,*intensity_wdgt,*rLow_wdgt,*rHigh_wdgt,*gLow_wdgt,*gHigh_wdgt,*bLow_wdgt,*bHigh_wdgt,*x_start,*y_start,*color_button,*bias_wdgt,*radius_wdgt,*error_output;
               
void generate(GtkWidget *genBn, gpointer data) {
    
    std::string imgName = gtk_entry_get_text(GTK_ENTRY(imgName_wdgt));
    int width = atoi((char *)gtk_entry_get_text(GTK_ENTRY(width_wdgt)));
    int height = atoi((char *)gtk_entry_get_text(GTK_ENTRY(height_wdgt))); 
    int x = atoi((char *)gtk_entry_get_text(GTK_ENTRY(x_start))); 
    int y = atoi((char *)gtk_entry_get_text(GTK_ENTRY(y_start))); 

    bool valid = true;

    gtk_label_set_text(GTK_LABEL(error_output),"");
    if(imgName == "") { 
        valid = false;
        gtk_label_set_text(GTK_LABEL(error_output),"Missing name of file");
    }
    else {
        if(width < 0 || height < 0){
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
        //converts the color button's color to 3 ints red, green, and blue;
        GdkRGBA *color = new GdkRGBA();
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(color_button), color);
        std::string temp_color = gdk_rgba_to_string(color);
        int initRed = (int)(stoi(temp_color.substr(temp_color.find_first_of('(')+1,temp_color.find_first_of(',')-temp_color.find_first_of('(')-1)));
        temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
        int initGreen = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(','))));
        temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
        int initBlue = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(')'))));

        PPM p(
            imgName,
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(intensity_wdgt))), 
            width,height,y*width+x,
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(rLow_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(rHigh_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(gLow_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(gHigh_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bLow_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bHigh_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(radius_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bias_wdgt))),
            initRed,initGreen,initBlue);


        //Timer t;

        //startPix(p,nxt,width,height,intensity,rRng,gRng,bRng,rOfst,gOfst,bOfst); //get user input on starting pixels and starts pixels;

        std::ofstream ppm(p.name+".ppm");
        ppm<<"P3 "<<p.width<<" "<<p.height<<" "<<p.intensity<<"\n";

        //random starting position
        bool last = false;
        if(p.pos <= -1) {
            p.pos = rand()%width*height;
        }
        p.locToPix();
        do {
            if(p.nxt.empty()) //only happen when nxt has no more pixels that need to be colored.
                last = true;
            if(!p.nxt.empty()) {
                int rando = rand()%p.nxt.size();
                for(int i=0;i<p.bias;i++)
                    if(rando)
                        rando = rand()%rando;
                std::swap(p.nxt[rando],p.nxt[p.nxt.size()-1]);
                p.nxt.pop_back();
            }
            p.locToPix();
        } while(!last);

        //printing arr p to ppm
        for (int i = 0; i < p.width*p.height; i++){
            ppm <<p.p[i].r<<" "
                <<p.p[i].g<<" "
                <<p.p[i].b<<" \t";
        }

        gtk_label_set_text(GTK_LABEL(error_output),(p.name+" has been successfully generated").c_str());
    }
    
    return;
}

int main(int argc, char **argv) {
    int row = 0;
    srand(time(NULL)); //init rand seed;
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *imgName_wdgt_label = gtk_label_new("Image Name:");
    imgName_wdgt = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt, 1, row++, 1, 1);

    GtkWidget *space0 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space0, 0, row++, 1, 1);

    GtkWidget *demensions_label = gtk_label_new("Dimensions  (Width,Height): ");
    width_wdgt = gtk_entry_new();
    height_wdgt = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), demensions_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_wdgt, 2, row++, 1, 1);

    GtkWidget *space1 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space1, 0, row++, 1, 1);

    GtkWidget *intensity_wdgt_directions_pt0 = gtk_label_new("Higher number for more precision");
    GtkWidget *intensity_wdgt_directions_pt1 = gtk_label_new(" (must be less than 65536)");
    GtkWidget *intensity_wdgt_label = gtk_label_new("Color intensity: ");
    intensity_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_directions_pt0, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_directions_pt1, 1, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_wdgt, 1, row++, 1, 1);
    
    GtkWidget *space2 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space2, 0, row++, 1, 1);

    GtkWidget *rng_label = gtk_label_new("Range of fluctuation");
    GtkWidget *rng_label_format = gtk_label_new("(minimum, maximum)");
    gtk_grid_attach(GTK_GRID(grid), rng_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rng_label_format, 1, row++, 1, 1);

    GtkWidget *rRng_label = gtk_label_new("Red Range: ");
    rLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    rHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), rRng_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rLow_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rHigh_wdgt, 2, row++, 1, 1);

    GtkWidget *gRng_label = gtk_label_new("Green Range: ");
    gLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    gHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), gRng_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gLow_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gHigh_wdgt, 2, row++, 1, 1);

    GtkWidget *bRng_label = gtk_label_new("Blue Range: ");
    bLow_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    bHigh_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), bRng_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bLow_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bHigh_wdgt, 2, row++, 1, 1);

    GtkWidget *space3 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space3, 0, row++, 1, 1);

    GtkWidget *starting_pnt_label_pt0 = gtk_label_new("Starting location");
    GtkWidget *starting_pnt_label_pt1 = gtk_label_new("-1 for random location");
    GtkWidget *starting_pnt_label = gtk_label_new("(x, y): ");
    x_start = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-1",2));
    y_start = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-1",2));
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label_pt0, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label_pt1, 1, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), starting_pnt_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_start, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_start, 2, row++, 1, 1);

    GtkWidget *space4 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space3, 0, row++, 1, 1);

    GtkWidget *color_label = gtk_label_new("Starting color: ");
    color_button = gtk_color_button_new();
    gtk_grid_attach(GTK_GRID(grid), color_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_button, 1, row++, 1, 1);

    GtkWidget *radius_label = gtk_label_new("Radius: ");
    bias_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    radius_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("1",1));
    gtk_grid_attach(GTK_GRID(grid), radius_label, 0, row,1,1);
    gtk_grid_attach(GTK_GRID(grid), radius_wdgt, 1, row,1,1);
    gtk_grid_attach(GTK_GRID(grid), bias_wdgt, 2, row++, 1, 1);

    GtkWidget *genBn = gtk_button_new_with_label("Generate");
    g_signal_connect(genBn, "clicked", G_CALLBACK(generate), NULL);
    gtk_grid_attach(GTK_GRID(grid), genBn, 0, row++, 1, 1);
    
    error_output = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), error_output, 0, row++, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}