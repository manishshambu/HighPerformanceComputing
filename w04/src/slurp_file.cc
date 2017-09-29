/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 */

#include "slurp_file.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
 * Consumes line `line_num` of file and put it into data vector. Line
 * numbering starts at zero.
 */
void slurp_file_line(const char* fname, int line_num, std::vector<data_t> *data){

    std::ifstream fh(fname, std::ios::in);

    if(!fh.good()){
        std::cerr<<"Cannot open file "<<fname<<std::endl;
        return;
    }

    std::string line;
    while ( getline(fh, line) && line_num>0) --line_num;

    data_t e;
    std::istringstream is(line);
    while (is.good()){
        is>>e;
        data->push_back(e);
    }

    fh.close();
}
