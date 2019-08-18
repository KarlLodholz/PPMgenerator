// #include <gtkmm.h>

// int main(int argc, char *argv[]) {
//     Gtk::Main kit(argc, argv);
//     Gtk::Window w;
//     Gtk::Box(Gtk::Orientation orientation = Gtk::ORIENTATION_HORIZONAL, int spacing = 0);
//     b
//     Gtk::Button b1("Here's");
//     Gtk::Button b2("no");

//     w.set_default_size(640,480);
//     w.set_title("PPM Generator");
//     w.set_position(Gtk::WIN_POS_CENTER);

//     b1.show();
//     w.add(b1);
//     b2.show();
//     w.add(b2);

//     Gtk::Main::run(w);
//     return 0;
// }

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

static GtkWidget *imgName,*xSz,*ySz,*intensity,*rLow,*rHigh,*gLow,*gHigh,*bLow,*bHigh,*x_start,*y_start;

void generate(GtkWidget *genBn, gpointer data) {
    // int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    // int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    // char buffer[32];
    // snprintf(buffer, sizeof(buffer), "result: %d", num1 + num2);

    // gtk_label_set_text(GTK_LABEL(result), buffer);
}

// gcc 007_gtk.c -o 007_gtk `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
int main(int argc, char **argv) {
    GtkWidget *window, *grid, *genBn;
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *imgName_label = gtk_label_new("Image Name:");
    imgName = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), imgName_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), imgName, 1, 0, 1, 1);

    GtkWidget *space0 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space0, 0, 1, 1, 1);

    GtkWidget *demensions_label = gtk_label_new("Dimensions  (Width,Height): ");
    xSz = gtk_entry_new();
    ySz = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), demensions_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), xSz, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ySz, 2, 2, 1, 1);

    GtkWidget *space1 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space1, 0, 3, 1, 1);

    GtkWidget *intensity_directions_pt0 = gtk_label_new("Higher number for more precision");
    GtkWidget *intensity_directions_pt1 = gtk_label_new(" (must be less than 65536)");
    GtkWidget *intensity_label = gtk_label_new("Color intensity: ");
    intensity = gtk_entry_new_with_buffer(gtk_entry_buffer_new("255",3));
    gtk_grid_attach(GTK_GRID(grid), intensity_directions_pt0, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_directions_pt1, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), intensity, 1, 5, 1, 1);
    
    GtkWidget *space2 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), space2, 0, 6, 1, 1);

    GtkWidget *rng_label = gtk_label_new("Range of fluctuation");
    GtkWidget *rng_label_format = gtk_label_new("(minimum, maximum)");
    gtk_grid_attach(GTK_GRID(grid), rng_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rng_label_format, 1, 7, 1, 1);

    GtkWidget *rRng_label = gtk_label_new("Red Range: ");
    rLow = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    rHigh = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), rRng_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rLow, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rHigh, 2, 8, 1, 1);

    GtkWidget *gRng_label = gtk_label_new("Green Range: ");
    gLow = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    gHigh = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), gRng_label, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gLow, 1, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gHigh, 2, 9, 1, 1);

    GtkWidget *bRng_label = gtk_label_new("Blue Range: ");
    bLow = gtk_entry_new_with_buffer(gtk_entry_buffer_new("-5",2));
    bHigh = gtk_entry_new_with_buffer(gtk_entry_buffer_new("5",1));
    gtk_grid_attach(GTK_GRID(grid), bRng_label, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bLow, 1, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bHigh, 2, 10, 1, 1);


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

    // number1 = gtk_entry_new();
    // gtk_grid_attach(GTK_GRID(grid), number1, 0, 0, 1, 1);

    // number2 = gtk_entry_new();
    // gtk_grid_attach(GTK_GRID(grid), number2, 1, 0, 1, 1);

    // genBn = gtk_button_new_with_label("Generate");
    // g_signal_connect(genBn, "clicked", G_CALLBACK(generate), NULL);
    // gtk_grid_attach(GTK_GRID(grid), genBn, 2, 0, 1, 1);

    // result = gtk_label_new("result:");
    // gtk_grid_attach(GTK_GRID(grid), result, 0, 1, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}