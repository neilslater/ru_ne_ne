// ext/co_ne_ne/ruby_module_co_ne_ne.h

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of narray helper functions
//

#ifndef RUBY_MODULE_CO_NE_NE_H
#define RUBY_MODULE_CO_NE_NE_H

#include <ruby.h>
#include "narray.h"
#include "core_narray.h"
#include "core_convolve.h"
#include "core_max_pool.h"
#include "ruby_module_transfer.h"
#include "ruby_class_mlp_layer.h"
#include "ruby_class_mlp_network.h"
#include "ruby_class_training_data.h"
#include "core_mt.h"
#include "core_qsort.h"

void init_module_co_ne_ne();

#endif
