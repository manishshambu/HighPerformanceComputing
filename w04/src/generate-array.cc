/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 */

#include <iostream>
#include <cstdlib> //rand
#include <cassert>

// expected arguments are number of lines and number of elements per line
int main(int argc, char **argv){

    assert (argc>2);
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);

    for (int l(0);l<k;++l){
        for (int w(0);w<n;++w)
            std::cout<<rand() % 1000<<" ";
        std::cout<<std::endl;
    }
}
