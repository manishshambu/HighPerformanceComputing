#include <vector>
#include <stddef.h>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <mpi.h>


typedef double data_t;

/**
* Returns the shuffling index set such that v[idx] is sorted
**/

void sort_index(const std::vector<size_t> &v, std::vector<size_t> &idx){

    size_t n(0);
    idx.resize(v.size());
    std::generate(idx.begin(), idx.end(), [&]{ return n++; });
    std::sort(idx.begin(), idx.end(), [&](size_t ii, size_t jj) { return v[ii] < v[jj]; } );
}

class sparse {
  public:
    enum storage_type {CSR, COO};
    std::vector<data_t> data;
    std::vector<size_t> column;
    std::vector<size_t> row;

    size_t m,n; /* (local) matrix dimensions */
    storage_type type;
    sparse(size_t m, size_t n, storage_type t = CSR);
    void transpose(sparse &B) const; /* returns transpose in B */
};

sparse::sparse(size_t m, size_t n, storage_type t) :
    m(m), n(n), type(t) {}

