/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 */

#include <vector>

typedef int data_t;

/*
 * Consumes line `line_num` of file and put it into data vector. Line
 * numbering starts at zero.
 */
void slurp_file_line(const char* fname, int line_num, std::vector<data_t> *data);
