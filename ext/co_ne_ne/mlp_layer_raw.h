// ext/con_ne_ne/mlp_layer_raw.h

#ifndef MLP_LAYER_RAW_H
#define MLP_LAYER_RAW_H

#include <ruby.h>
#include "narray.h"
#include "core_mt.h"
#include "ruby_module_transfer.h"

typedef struct _mlp_layer_raw {
    int num_inputs;
    int num_outputs;
    transfer_type transfer_fn;
    VALUE narr_input;
    VALUE narr_output;
    VALUE narr_weights;
    VALUE input_layer;
    VALUE output_layer;
    VALUE narr_output_deltas;
    VALUE narr_weights_last_deltas;
    VALUE narr_output_slope;
  } MLP_Layer;

MLP_Layer *create_mlp_layer_struct();

void destroy_mlp_layer_struct( MLP_Layer *mlp_layer );

// Called by Ruby's GC, we have to mark all child objects that could be in Ruby
// space, so that they don't get deleted.
void mark_mlp_layer_struct( MLP_Layer *mlp_layer );

// Note this isn't called from initialize_copy, it's for internal copies
MLP_Layer *copy_mlp_layer_struct( MLP_Layer *orig );

void mlp_layer_struct_create_arrays( MLP_Layer *mlp_layer );

void mlp_layer_struct_init_weights( MLP_Layer *mlp_layer, float min, float max );

void mlp_layer_struct_use_weights( MLP_Layer *mlp_layer, VALUE weights );

void mlp_layer_run( MLP_Layer *mlp_layer );

void activate_nn_layer_raw( int in_size, int out_size, float *in_ptr, float *weights, float *out_ptr );

float ms_error_raw( int out_size, float *out_ptr, float *target_ptr );

void calc_output_deltas_raw( int out_size, float *out_ptr, float *out_slope_ptr, float *target_ptr, float *out_delta_ptr );

void backprop_deltas_raw( int in_size, int out_size, float *in_deltas, float *in_slopes, float *weights, float *out_deltas );

void mlp_layer_backprop( MLP_Layer *mlp_layer, MLP_Layer *mlp_layer_input );

void mlp_layer_struct_update_weights( MLP_Layer *mlp_layer, float eta, float m );

void update_weights_raw( float eta, float m, int in_size, int out_size, float *inputs, float *weights, float *weights_last_deltas, float *output_deltas);

#endif
