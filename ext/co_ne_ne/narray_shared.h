// ext/co_ne_ne/narray_shared.h

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Declarations of narray helper functions
//

#ifndef CONVOLVER_NARRAY_SHARED_H
#define CONVOLVER_NARRAY_SHARED_H

#include <ruby.h>
#include "narray.h"

// This is copied from na_array.c, with safety checks and temp vars removed
int na_quick_idxs_to_pos( int rank, int *shape, int *idxs );

// This is inverse of above
void na_quick_pos_to_idxs( int rank, int *shape, int pos, int *idxs );

#endif
