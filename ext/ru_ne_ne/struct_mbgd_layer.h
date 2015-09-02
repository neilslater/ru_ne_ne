// ext/ru_ne_ne/struct_mbgd_layer.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for MBGDLayer and declarations for its memory management
//

#ifndef STRUCT_TRAINER_BP_LAYER_H
#define STRUCT_TRAINER_BP_LAYER_H

#include <ruby.h>
#include "narray.h"
#include "struct_layer_ff.h"
#include "core_objective_functions.h"

typedef enum {GDACCEL_TYPE_NONE, GDACCEL_TYPE_MOMENTUM, GDACCEL_TYPE_RMSPROP} gd_accel_type;

typedef struct _mbgd_layer_raw {
  int num_inputs;
  int num_outputs;
  VALUE narr_de_dz;
  float *de_dz;
  VALUE narr_de_da;
  float *de_da;
  VALUE narr_de_dw;
  float *de_dw;
  VALUE narr_de_dw_stats_a;
  float *de_dw_stats_a;
  VALUE narr_de_dw_stats_b;
  float *de_dw_stats_b;
  float learning_rate;
  gd_accel_type gd_accel_type;
  float gd_accel_rate;
  float max_norm;
  float weight_decay;
  } MBGDLayer;

MBGDLayer *mbgd_layer__create();

void mbgd_layer__init( MBGDLayer *mbgd_layer, int num_inputs, int num_outputs );

void mbgd_layer__destroy( MBGDLayer *mbgd_layer );

void mbgd_layer__gc_mark( MBGDLayer *mbgd_layer );

void mbgd_layer__deep_copy( MBGDLayer *mbgd_layer_copy, MBGDLayer *mbgd_layer_orig );

MBGDLayer * mbgd_layer__clone( MBGDLayer *mbgd_layer_orig );

void mbgd_layer__start_batch( MBGDLayer *mbgd_layer );

void mbgd_layer__backprop_for_output_layer( MBGDLayer *mbgd_layer, Layer_FF *layer_ff,
      float *input, float *output, float *target, objective_type o );

void mbgd_layer__backprop_for_mid_layer( MBGDLayer *mbgd_layer, Layer_FF *layer_ff,
      float *input, float *output, float *upper_de_da );

void mbgd_layer__finish_batch( MBGDLayer *mbgd_layer, Layer_FF *layer_ff );

#endif