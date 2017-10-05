/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 */

/* @input n the size of the data vector x
 * @input x data vector
 * @input num_bins the number of bins to use
 * @output bin_bdry array of size num_bins+1 holding the left and
 *         right limit of each bin
 * @output bin_count array of size num_bins holding the count of
 *         items in each bin.
 *
 * For both output arrays, memory is allocated by the histogram
 * function and the caller is responsible for deleting.
 */

#include <cstddef>

void histogram(size_t n, const double *x, int num_bins, /* inputs  */
	           double **bin_bdry, size_t **bin_count);  /* outputs */
