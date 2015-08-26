// ext/ru_ne_ne/struct_trainer_bp_layer.c

#include "struct_trainer_bp_layer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for TrainerBPLayer memory management
//

TrainerBPLayer *trainer_bp_layer__create() {
  TrainerBPLayer *trainer_bp_layer;
  trainer_bp_layer = xmalloc( sizeof(TrainerBPLayer) );
  trainer_bp_layer->num_inputs = 0;
  trainer_bp_layer->num_outputs = 0;
  trainer_bp_layer->narr_de_dz = Qnil;
  trainer_bp_layer->de_dz = NULL;
  trainer_bp_layer->narr_de_da = Qnil;
  trainer_bp_layer->de_da = NULL;
  trainer_bp_layer->narr_de_dw = Qnil;
  trainer_bp_layer->de_dw = NULL;
  trainer_bp_layer->narr_de_dw_stats_a = Qnil;
  trainer_bp_layer->de_dw_stats_a = NULL;
  trainer_bp_layer->narr_de_dw_stats_b = Qnil;
  trainer_bp_layer->de_dw_stats_b = NULL;
  trainer_bp_layer->learning_rate = 0.01;
  trainer_bp_layer->gd_accel_type = GDACCEL_TYPE_NONE;
  trainer_bp_layer->gd_accel_rate = 0.9;
  trainer_bp_layer->max_norm = 0.0;
  trainer_bp_layer->weight_decay = 0.0;
  return trainer_bp_layer;
}

void trainer_bp_layer__init( TrainerBPLayer *trainer_bp_layer, int num_inputs, int num_outputs ) {
  int i;
  int shape[2];
  struct NARRAY *narr;
  float *narr_de_dz_ptr;
  float *narr_de_da_ptr;
  float *narr_de_dw_ptr;
  float *narr_de_dw_stats_a_ptr;
  float *narr_de_dw_stats_b_ptr;

  trainer_bp_layer->num_inputs = num_inputs;

  trainer_bp_layer->num_outputs = num_outputs;

  shape[0] = num_outputs;
  trainer_bp_layer->narr_de_dz = na_make_object( NA_SFLOAT, 1, shape, cNArray );
  GetNArray( trainer_bp_layer->narr_de_dz, narr );
  narr_de_dz_ptr = (float*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_de_dz_ptr[i] = 0.0;
  }
  trainer_bp_layer->de_dz = (float *) narr->ptr;

  shape[0] = num_inputs;
  trainer_bp_layer->narr_de_da = na_make_object( NA_SFLOAT, 1, shape, cNArray );
  GetNArray( trainer_bp_layer->narr_de_da, narr );
  narr_de_da_ptr = (float*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_de_da_ptr[i] = 0.0;
  }
  trainer_bp_layer->de_da = (float *) narr->ptr;

  shape[0] = num_inputs + 1;
  shape[1] = num_outputs;
  trainer_bp_layer->narr_de_dw = na_make_object( NA_SFLOAT, 2, shape, cNArray );
  GetNArray( trainer_bp_layer->narr_de_dw, narr );
  narr_de_dw_ptr = (float*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_de_dw_ptr[i] = 0.0;
  }
  trainer_bp_layer->de_dw = (float *) narr->ptr;

  trainer_bp_layer->narr_de_dw_stats_a = na_make_object( NA_SFLOAT, 2, shape, cNArray );
  GetNArray( trainer_bp_layer->narr_de_dw_stats_a, narr );
  narr_de_dw_stats_a_ptr = (float*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_de_dw_stats_a_ptr[i] = 0.0;
  }
  trainer_bp_layer->de_dw_stats_a = (float *) narr->ptr;

  trainer_bp_layer->narr_de_dw_stats_b = na_make_object( NA_SFLOAT, 2, shape, cNArray );
  GetNArray( trainer_bp_layer->narr_de_dw_stats_b, narr );
  narr_de_dw_stats_b_ptr = (float*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_de_dw_stats_b_ptr[i] = 0.0;
  }
  trainer_bp_layer->de_dw_stats_b = (float *) narr->ptr;

  return;
}

void trainer_bp_layer__destroy( TrainerBPLayer *trainer_bp_layer ) {
  xfree( trainer_bp_layer );
  return;
}

void trainer_bp_layer__gc_mark( TrainerBPLayer *trainer_bp_layer ) {
  rb_gc_mark( trainer_bp_layer->narr_de_dz );
  rb_gc_mark( trainer_bp_layer->narr_de_da );
  rb_gc_mark( trainer_bp_layer->narr_de_dw );
  rb_gc_mark( trainer_bp_layer->narr_de_dw_stats_a );
  rb_gc_mark( trainer_bp_layer->narr_de_dw_stats_b );
  return;
}

void trainer_bp_layer__deep_copy( TrainerBPLayer *trainer_bp_layer_copy, TrainerBPLayer *trainer_bp_layer_orig ) {
  struct NARRAY *narr;

  trainer_bp_layer_copy->num_inputs = trainer_bp_layer_orig->num_inputs;
  trainer_bp_layer_copy->num_outputs = trainer_bp_layer_orig->num_outputs;
  trainer_bp_layer_copy->learning_rate = trainer_bp_layer_orig->learning_rate;
  trainer_bp_layer_copy->gd_accel_type = trainer_bp_layer_orig->gd_accel_type;
  trainer_bp_layer_copy->gd_accel_rate = trainer_bp_layer_orig->gd_accel_rate;
  trainer_bp_layer_copy->max_norm = trainer_bp_layer_orig->max_norm;
  trainer_bp_layer_copy->weight_decay = trainer_bp_layer_orig->weight_decay;

  trainer_bp_layer_copy->narr_de_dz = na_clone( trainer_bp_layer_orig->narr_de_dz );
  GetNArray( trainer_bp_layer_copy->narr_de_dz, narr );
  trainer_bp_layer_copy->de_dz = (float *) narr->ptr;

  trainer_bp_layer_copy->narr_de_da = na_clone( trainer_bp_layer_orig->narr_de_da );
  GetNArray( trainer_bp_layer_copy->narr_de_da, narr );
  trainer_bp_layer_copy->de_da = (float *) narr->ptr;

  trainer_bp_layer_copy->narr_de_dw = na_clone( trainer_bp_layer_orig->narr_de_dw );
  GetNArray( trainer_bp_layer_copy->narr_de_dw, narr );
  trainer_bp_layer_copy->de_dw = (float *) narr->ptr;

  trainer_bp_layer_copy->narr_de_dw_stats_a = na_clone( trainer_bp_layer_orig->narr_de_dw_stats_a );
  GetNArray( trainer_bp_layer_copy->narr_de_dw_stats_a, narr );
  trainer_bp_layer_copy->de_dw_stats_a = (float *) narr->ptr;

  trainer_bp_layer_copy->narr_de_dw_stats_b = na_clone( trainer_bp_layer_orig->narr_de_dw_stats_b );
  GetNArray( trainer_bp_layer_copy->narr_de_dw_stats_b, narr );
  trainer_bp_layer_copy->de_dw_stats_b = (float *) narr->ptr;

  return;
}

TrainerBPLayer * trainer_bp_layer__clone( TrainerBPLayer *trainer_bp_layer_orig ) {
  TrainerBPLayer * trainer_bp_layer_copy = trainer_bp_layer__create();
  trainer_bp_layer__deep_copy( trainer_bp_layer_copy, trainer_bp_layer_orig );
  return trainer_bp_layer_copy;
}

void trainer_bp_layer__start_batch( TrainerBPLayer *trainer_bp_layer ) {
  int i,t = (trainer_bp_layer->num_inputs + 1 ) * trainer_bp_layer->num_outputs;
  float *de_dw = trainer_bp_layer->de_dw;
  for( i = 0; i < t; i++ ) {
    de_dw[i] = 0.0;
  }
  return;
}

void increment_de_dw_from_de_dz( int in_size, int out_size, float *inputs, float *de_dw, float *de_dz) {
  int i,j, offset;

  // If j were the inner loop, this might be able to use SIMD
  for ( j = 0; j < out_size; j++ ) {
    offset = j * ( in_size + 1 );
    for ( i = 0; i < in_size; i++ ) {
      de_dw[ offset + i ] += de_dz[j] * inputs[i];
      // TODO: We could do de_da[i] += de_dz[j] * weights[ offset + i ]; here?
      // measure best combination . . .
    }
    // For the bias, we have no input value
    de_dw[ offset + in_size ] += de_dz[j];
  }

  return;
}

// This "drops down" one layer calculating de_da for *inputs* to a layer
void calc_de_da_from_de_dz( int in_size, int out_size, float *weights, float *de_da, float *de_dz) {
  int i,j;
  float t;

  for ( i = 0; i < in_size; i++ ) {
    t = 0.0;
    for ( j = 0; j < out_size; j++ ) {
      t += de_dz[j] * weights[ j * ( in_size + 1 ) + i ];
    }
    de_da[i] = t;
  }

  return;
}

void trainer_bp_layer__backprop_for_output_layer( TrainerBPLayer *trainer_bp_layer, Layer_FF *layer_ff,
      float *input, float *output, float *target, objective_type o ) {

  de_dz_from_objective_and_transfer( o,
      layer_ff->transfer_fn,
      trainer_bp_layer->num_outputs,
      output,
      target,
      trainer_bp_layer->de_dz );

  increment_de_dw_from_de_dz( trainer_bp_layer->num_inputs,
      trainer_bp_layer->num_outputs,
      input,
      trainer_bp_layer->de_dw,
      trainer_bp_layer->de_dz );

  // TODO: Either combine for speed with incr_de_dw *or* make it optional (not required in first layer)
  calc_de_da_from_de_dz( trainer_bp_layer->num_inputs,
      trainer_bp_layer->num_outputs,
      layer_ff->weights,
      trainer_bp_layer->de_da,
      trainer_bp_layer->de_dz );

  return;
}

void  de_dz_from_upper_de_da( transfer_type t, int out_size, float *output, float *de_da, float *de_dz ) {
  int i;

  // This stores da_dz . . .
  transfer_bulk_derivative_at( t, out_size, output, de_dz );
  // de_dz = de_da * da_dz
  for ( i = 0; i < out_size; i++ ) {
    de_dz[i] *= de_da[i];
  }

  return;
}

void trainer_bp_layer__backprop_for_mid_layer( TrainerBPLayer *trainer_bp_layer, Layer_FF *layer_ff,
      float *input, float *output, float *upper_de_da ) {

  de_dz_from_upper_de_da( layer_ff->transfer_fn,
      trainer_bp_layer->num_outputs,
      output,
      upper_de_da,
      trainer_bp_layer->de_dz );

  increment_de_dw_from_de_dz( trainer_bp_layer->num_inputs,
      trainer_bp_layer->num_outputs,
      input,
      trainer_bp_layer->de_dw,
      trainer_bp_layer->de_dz );

  // TODO: Either combine for speed with incr_de_dw *or* make it optional (not required in first layer)
  calc_de_da_from_de_dz( trainer_bp_layer->num_inputs,
      trainer_bp_layer->num_outputs,
      layer_ff->weights,
      trainer_bp_layer->de_da,
      trainer_bp_layer->de_dz );

  return;
}
