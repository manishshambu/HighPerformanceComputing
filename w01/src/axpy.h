/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 * @brief function delcration for axpy()
 */

#include <cstddef>
#include <stdexcept>

void axpy(size_t n, double a, const double *x, double *y) throw (std::runtime_error);
