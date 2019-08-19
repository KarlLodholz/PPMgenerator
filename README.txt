This program is a ppm generator that will generate an image from parameters such as the red green and blue fluctuation, as well as the the starting points' color and position.

To use the GUI interface you will need gtk+3.0, however you can operate the program without
  For GUI: Compile with gtk+-3.0
    c++ ppm.cpp -o ppm `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
    ./ppm
  For terminal interface
    c++ main.cpp -o ppm
    ./ppm
