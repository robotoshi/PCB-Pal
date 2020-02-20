//
//  PCB_Pal.h
//  PCB-Pal
//
//  Created by Patrick Willmann on 2020-02-03.
//  Copyright © 2020 ENEL 400 - Group 13. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef PCB_Pal_h
#define PCB_Pal_h

struct coord {
    
    string type;
    double trace_thickness;
    double x1;
    double y1;
    double x2;
    double y2;
    
};

//Scrapes all line data from a BRD file and places it into an array of structs, each of which
//contains a start and end X & Y coordinates in double format.
class BRD_Scraper {
    
    private:
    string filename;
    int file_length;
    string units;
    double x_max;
    double y_max;
    string temp_line;
    coord * list;
    
    public:
    BRD_Scraper (string filename);
    ~BRD_Scraper ();
    void read_line(int line_num);
    coord * place_xy();

};

//Takes an array of type coord and generates a print file to send to Arduino. Adds relevant header information.
class file_gen {
    
    private:
    double x_length;
    double y_length;
    

    public:
    void file_out();
    
};

#endif /* PCB_Pal_h */
