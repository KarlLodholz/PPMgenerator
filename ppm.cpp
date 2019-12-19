#include "ppm.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "timer.h"

// compile with
// c++ ppm.cpp -o ppm `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

//////////////////////////////////////////  -TO DO-  /////////////////////////////////////////////
//cap range for how much each color can change max outside of the random aspect
//look into animation of a picture
//////////////////////////////////////////////////////////////////////////////////////////////////

static GtkWidget *imgName_wdgt,*width_wdgt,*height_wdgt,*intensity_wdgt,*rLow_wdgt,*rHigh_wdgt,*gLow_wdgt,*gHigh_wdgt,*bLow_wdgt,*bHigh_wdgt,*rFloor_wdgt,*rCeiling_wdgt,*gFloor_wdgt,*gCeiling_wdgt,*bFloor_wdgt,*bCeiling_wdgt,*x_start,*y_start,*color_button,*bias_wdgt,*growth_radius_wdgt,*color_radius_wdgt,*error_output;
               
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
        int init_red = (int)(stoi(temp_color.substr(temp_color.find_first_of('(')+1,temp_color.find_first_of(',')-temp_color.find_first_of('(')-1)));
        temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
        int init_green = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(','))));
        temp_color = temp_color.substr(temp_color.find_first_of(',')+1,std::string::npos);
        int init_blue = (int)(stoi(temp_color.substr(0,temp_color.find_first_of(')'))));

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
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(rCeiling_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(rFloor_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(gCeiling_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(gFloor_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bCeiling_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bFloor_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(color_radius_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(growth_radius_wdgt))),
            atoi((char *)gtk_entry_get_text(GTK_ENTRY(bias_wdgt))),
            init_red,init_green,init_blue);


        Timer t;

        std::ofstream ppm(p.name+".ppm");
        ppm<<"P3 "<<p.width<<" "<<p.height<<" "<<p.intensity<<"\n";

        //random starting position
        bool last = false;
        if(p.pos <= -1) {
            p.pos = rand()%(width*height);
        }
        p.init_pos = p.pos;
        p.get_neighbor_color();
        do {
            //for (int i = 0; i < p.width*p.height; i++) std::cout<<p.p[i].r<<" "<<p.p[i].g<<" "<<p.p[i].b<<" \t"; std::cout<<std::endl;
            if(p.nxt.empty()) //only happen when nxt has no more pixels that need to be colored.
                last = true;
            if(!p.nxt.empty()) {
                int rando = rand()%p.nxt.size();
                for(int i=0;i<p.bias;i++)
                    if(rando)
                        rando = rand()%rando;
                p.pos = p.nxt[rando];
                std::swap(p.nxt[rando],p.nxt[p.nxt.size()-1]);
                p.nxt.pop_back();
            }
            p.get_neighbor_color();
        } while(!last);

        //printing arr p to ppm
        for (int i = 0; i < p.width*p.height; i++){
            ppm <<p.p[i].r<<" "
                <<p.p[i].g<<" "
                <<p.p[i].b<<" \t";
        }

        //settings file
        std::ofstream ppm_settings(p.name+".txt");
        ppm_settings<<"//settings"
            <<"\nname:"<<p.name
            <<"\ndimensions:"<<p.width<<" "<<p.height
            <<"\nintensity:"<<p.intensity
            <<"\nrH-rL:"<<p.get_rHigh()<<" "<<p.get_rLow()
            <<"\ngH-gL:"<<p.get_gHigh()<<" "<<p.get_gLow()
            <<"\nbH-bL:"<<p.get_bHigh()<<" "<<p.get_bLow()
            <<"\nrF-rC:"<<p.get_rFloor()<<" "<<p.get_rCeiling()
            <<"\ngF-gC:"<<p.get_gFloor()<<" "<<p.get_gCeiling()
            <<"\nbF-bC:"<<p.get_bFloor()<<" "<<p.get_bCeiling()
            <<"\ninit_pos:"<<p.init_pos%p.width<<" "<<p.init_pos/p.width
            <<"\ninit_color:"<<p.get_init_red()<<" "<<p.get_init_green()<<" "<<p.get_init_blue()
            <<"\ncolor_radius:"<<p.get_color_radius()
            <<"\ngrowth_bias:"<<p.get_growth_bias();

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
    imgName_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("test",4));
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), imgName_wdgt, 1, row++, 1, 1);

    GtkWidget *space0 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space0, 0, row++, 1, 1);

    GtkWidget *demensions_label = gtk_label_new("Dimensions  (Width,Height): ");
    width_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("1920",4));
    height_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("1080",4));
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
    
    GtkWidget *color_max_min_label_pt0 = gtk_label_new("Color Floors & Ceilings");
    GtkWidget *color_max_min_label_pt1 = gtk_label_new("(floor, ceiling)");
    GtkWidget *color_max_min_label_pt2 = gtk_label_new("between 0 and 255");
    GtkWidget *color_max_min_red_label = gtk_label_new("Red:");
    rFloor_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    rCeiling_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    GtkWidget *color_max_min_green_label = gtk_label_new("Green:");
    gFloor_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    gCeiling_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    GtkWidget *color_max_min_blue_label = gtk_label_new("Blue:");
    bFloor_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    bCeiling_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    gtk_grid_attach(GTK_GRID(grid), color_max_min_label_pt0, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_max_min_label_pt1, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_max_min_label_pt2, 2, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_max_min_red_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rFloor_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rCeiling_wdgt, 2, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_max_min_green_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gFloor_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gCeiling_wdgt, 2, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_max_min_blue_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bFloor_wdgt, 1, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bCeiling_wdgt, 2, row++, 1, 1);

    GtkWidget *space4 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space4, 0, row++, 1, 1);

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

    GtkWidget *space5 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space5, 0, row++, 1, 1);

    GtkWidget *color_label = gtk_label_new("Starting color: ");
    GtkWidget *growth_bias_label = gtk_label_new("Growth Bias: ");
    GtkWidget *growth_radius_label = gtk_label_new("Growth Radius: ");
    GtkWidget *color_radius_label = gtk_label_new("Color Radius: ");
    color_button = gtk_color_button_new();

    bias_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("0",1));
    growth_radius_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("1",1));
    color_radius_wdgt = gtk_entry_new_with_buffer(gtk_entry_buffer_new("1",1));
    gtk_grid_attach(GTK_GRID(grid), color_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_button, 1, row++, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), growth_radius_label, 0, row,1,1);
    gtk_grid_attach(GTK_GRID(grid), growth_radius_wdgt, 1, row++,1,1);
    gtk_grid_attach(GTK_GRID(grid), color_radius_label, 0, row,1,1);
    gtk_grid_attach(GTK_GRID(grid), color_radius_wdgt, 1, row++,1,1);
    gtk_grid_attach(GTK_GRID(grid), growth_bias_label,0, row, 1,1);
    gtk_grid_attach(GTK_GRID(grid), bias_wdgt, 1, row++, 1, 1);

    GtkWidget *genBn = gtk_button_new_with_label("Generate");
    g_signal_connect(genBn, "clicked", G_CALLBACK(generate), NULL);
    gtk_grid_attach(GTK_GRID(grid), genBn, 0, row++, 1, 1);
    
    error_output = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), error_output, 0, row++, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}