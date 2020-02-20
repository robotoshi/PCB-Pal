//
//  main.cpp
//  PCB-Pal
//
//  Created by Patrick Willmann on 2020-02-03.
//  Copyright © 2020 ENEL 400 - Group 13. All rights reserved.
//

#include "PCB_Pal.h"

int main(int argc, const char * argv[]) {
    string filename_in;
    cout << "Enter Filename" << endl;
    cin >> filename_in;
    BRD_Scraper scrape (filename_in);
    
    return 0;
}
