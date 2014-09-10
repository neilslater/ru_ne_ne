// ext/co_ne_ne/struct_training_data.c

#include "struct_training_data.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating TrainingData structs
//

TrainingData *p_training_data_create() {
  TrainingData *training_data;
  training_data = xmalloc( sizeof(TrainingData) );
  training_data->narr_inputs = Qnil;
  training_data->narr_outputs = Qnil;
  training_data->random_sequence = 0;
  training_data->input_item_size = 0;
  training_data->output_item_size = 0;
  training_data->pos_idx = NULL;
  training_data->current_pos = 0;
  training_data->num_items = 0;
  return training_data;
}

void p_training_data_init( TrainingData *training_data, int input_rank, int *input_shape,
      int output_rank, int *output_shape, int num_items ) {
  int *tmp_shape, i, size, *pos;
  struct NARRAY *narr;

  tmp_shape = ALLOC_N( int, input_rank + 1 );
  size = 1;
  for( i = 0; i < input_rank; i++ ) {
    tmp_shape[i] = input_shape[i];
    size *= input_shape[i];
  }
  tmp_shape[input_rank] = num_items;
  training_data->narr_inputs = na_make_object( NA_SFLOAT, input_rank + 1, tmp_shape, cNArray );
  training_data->input_item_size = size;
  GetNArray( training_data->narr_inputs, narr );
  na_sfloat_set( narr->total, (float*) narr->ptr, (float) 0.0 );
  xfree( tmp_shape );

  tmp_shape = ALLOC_N( int, output_rank + 1 );
  size = 1;
  for( i = 0; i < output_rank; i++ ) {
    tmp_shape[i] = output_shape[i];
    size *= output_shape[i];
  }
  tmp_shape[output_rank] = num_items;
  training_data->narr_outputs = na_make_object( NA_SFLOAT, output_rank + 1, tmp_shape, cNArray );
  training_data->output_item_size = size;
  GetNArray( training_data->narr_outputs, narr );
  na_sfloat_set( narr->total, (float*) narr->ptr, (float) 0.0 );
  xfree( tmp_shape );

  training_data->num_items = num_items;

  pos = ALLOC_N( int, num_items );
  for( i = 0; i < num_items; i++ ) {
    pos[i] = i;
  }
  training_data->pos_idx = pos;
  training_data->current_pos = 0;

  return;
}

float *p_training_data_current_input( TrainingData *training_data ) {
  int i;
  struct NARRAY *narr;
  GetNArray( training_data->narr_inputs, narr );
  i = training_data->pos_idx[ training_data->current_pos ];
  return (float*) narr->ptr[ training_data->input_item_size * i ];
}

float *p_training_data_current_output( TrainingData *training_data ) {
  int i;
  struct NARRAY *narr;
  GetNArray( training_data->narr_outputs, narr );
  i = training_data->pos_idx[ training_data->current_pos ];
  return (float*) narr->ptr[ training_data->output_item_size * i ];
}

void p_training_data_next( TrainingData *training_data ) {
  training_data->current_pos++;
  training_data->current_pos %= training_data->num_items;
  return;
}

// Init assuming 1-dimensional arrays for input and output
void p_training_data_init_simple( TrainingData *training_data, int input_size,
      int output_size, int num_items ) {
  int in_shape[1], out_shape[1];
  in_shape[0] = input_size;
  out_shape[0] = output_size;

  p_training_data_init( training_data, 1, in_shape, 1, out_shape, num_items );
  return;
}

void p_training_data_destroy( TrainingData *training_data ) {
  xfree( training_data->pos_idx );
  xfree( training_data );

  // No need to free NArrays - they will be handled by Ruby's GC, and may still be reachable
  return;
}

// Called by Ruby's GC, we have to mark all child objects that could be in Ruby
// space, so that they don't get deleted.
void p_training_data_gc_mark( TrainingData *training_data ) {
  rb_gc_mark( training_data->narr_inputs );
  rb_gc_mark( training_data->narr_outputs );
  return;
}

void p_training_data_init_from_narray( TrainingData *training_data, VALUE inputs, VALUE outputs ) {
  int *tmp_shape, i, size, *pos, num_items;
  struct NARRAY *na_inputs;
  struct NARRAY *na_outputs;

  training_data->narr_inputs = inputs;
  training_data->narr_outputs = outputs;
  GetNArray( training_data->narr_inputs, na_inputs );
  GetNArray( training_data->narr_outputs, na_outputs );

  tmp_shape = na_inputs->shape;
  size = 1;
  for( i = 0; i < na_inputs->rank - 1; i++ ) {
    size *= tmp_shape[i];
  }
  num_items = tmp_shape[ na_inputs->rank - 1 ];
  training_data->input_item_size = size;

  tmp_shape = na_outputs->shape;
  size = 1;
  for( i = 0; i < na_outputs->rank - 1; i++ ) {
    size *= tmp_shape[i];
  }
  training_data->output_item_size = size;

  pos = ALLOC_N( int, num_items );
  for( i = 0; i < num_items; i++ ) {
    pos[i] = i;
  }
  training_data->pos_idx = pos;
  training_data->current_pos = 0;
  training_data->num_items = num_items;
  return;
}