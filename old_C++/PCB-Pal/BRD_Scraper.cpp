//
//  BRD_Scraper.cpp
//  PCB-Pal
//
//  Created by Patrick Willmann on 2020-02-03.
//  Copyright © 2020 ENEL 400 - Group 13. All rights reserved.
//

#include <stdio.h>
#include "PCB_Pal.h"

BRD_Scraper :: BRD_Scraper (string filename) {
    filename = filename;
    
    ifstream input_brd (filename.c_str(), ios::in);
    cout << "Loading .brd file" << endl;
    if (input_brd.fail()) {
        cout << "Failed to load file." << endl;
        exit(1);
    }
    else {
        input_brd.seekg (0, input_brd.end);
        file_length = ((int)input_brd.tellg());
        input_brd.seekg (0, input_brd.beg);
        cout << "File length is " << file_length << endl;
        
    }
    input_brd.close();
}

BRD_Scraper :: ~BRD_Scraper () {
    //Delete everything - make me last
}

void BRD_Scraper :: read_line(int line_num) {
    //Implementation here
    
}

coord * BRD_Scraper :: place_xy() {
    //Implementation here
    return list;
}
