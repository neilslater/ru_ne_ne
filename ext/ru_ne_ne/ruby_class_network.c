// ext/ru_ne_ne/ruby_class_network.c

#include "ruby_class_network.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_network.c
//

inline VALUE network_as_ruby_class( Network *network , VALUE klass ) {
  return Data_Wrap_Struct( klass, network__gc_mark, network__destroy, network );
}

VALUE network_alloc(VALUE klass) {
  return network_as_ruby_class( network__create(), klass );
}

inline Network *get_network_struct( VALUE obj ) {
  Network *network;
  Data_Get_Struct( obj, Network, network );
  return network;
}

void assert_value_wraps_network( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)network__destroy) {
    rb_raise( rb_eTypeError, "Expected a Network object, but got something else" );
  }
}

/* Document-class: RuNeNe::Network
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( layers )
 * Creates a new ...
 * @param [Array<RuNeNe::Layer::Feedforward>] layers ...
 * @return [RuNeNe::Network] new ...
 */
VALUE network_rbobject__initialize( VALUE self, VALUE rv_layers ) {
  Network *network = get_network_struct( self );
  // This stack-based var avoids memory leaks from alloc which might not be freed on error
  VALUE layers[100];
  volatile VALUE current_layer;
  int i, n;

  Check_Type( rv_layers, T_ARRAY );

  n = FIX2INT( rb_funcall( rv_layers, rb_intern("count"), 0 ) );
  if ( n < 1 ) {
    rb_raise( rb_eArgError, "no layers in network" );
  }
  if ( n > 100 ) {
    rb_raise( rb_eArgError, "too many layers in network" );
  }

  for ( i = 0; i < n; i++ ) {
    current_layer = rb_ary_entry( rv_layers, i );
    // TODO: Accept more than one definition of layer (e.g. orig object, hash). Support
    //       multiple layer types in theory.
    assert_value_wraps_layer_ff( current_layer );
    layers[i] = current_layer;
  }

  network__init( network, n, layers);

  return self;
}

/* @overload clone
 * When cloned, the returned Network has deep copies of C data.
 * @return [RuNeNe::Network] new
 */
VALUE network_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  Network *network_copy;
  Network *network_orig;

  if (copy == orig) return copy;
  network_orig = get_network_struct( orig );
  network_copy = get_network_struct( copy );

  network__deep_copy( network_copy, network_orig );

  return copy;
}

/* @!attribute [r] layers
 * Description goes here
 * @return [Array<RuNeNe::Layer::Feedforward>]]
 */
VALUE network_rbobject__get_layers( VALUE self ) {
  Network *network = get_network_struct( self );
  int i;

  volatile VALUE rv_layers = rb_ary_new2( network->num_layers );
  for ( i = 0; i < network->num_layers; i++ ) {
    rb_ary_store( rv_layers, i, network->layers[i] );
  }

  return rv_layers;
}

/* @!attribute [r] num_layers
 * Description goes here
 * @return [Integer]
 */
VALUE network_rbobject__get_num_layers( VALUE self ) {
  Network *network = get_network_struct( self );
  return INT2NUM( network->num_layers );
}

/* @!attribute [r] num_inputs
 * Description goes here
 * @return [Integer]
 */
VALUE network_rbobject__get_num_inputs( VALUE self ) {
  Network *network = get_network_struct( self );
  return INT2NUM( network->num_inputs );
}

/* @!attribute [r] num_outputs
 * Description goes here
 * @return [Integer]
 */
VALUE network_rbobject__get_num_outputs( VALUE self ) {
  Network *network = get_network_struct( self );
  return INT2NUM( network->num_outputs );
}


/* @overload init_weights( mult = 1.0 )
 * Initialises weights in all layers.
 * @param [Float] mult optional size factor
 * @return [RuNeNe::Network] self
 */
VALUE network_rbobject__init_weights( int argc, VALUE* argv, VALUE self ) {
  Network *network = get_network_struct( self );
  VALUE rv_mult;
  Layer_FF *layer_ff;
  float m = 1.0;
  int i, j, t;
  struct NARRAY *narr;

  rb_scan_args( argc, argv, "01", &rv_mult );
  if ( ! NIL_P( rv_mult ) ) {
    m = NUM2FLT( rv_mult );
  }

  for ( i = 0; i < network->num_layers; i++ ) {
    // TODO: This only works for Layer_FF layers, we need a more flexible system
    Data_Get_Struct( network->layers[i], Layer_FF, layer_ff );

    layer_ff__init_weights( layer_ff );

    if ( m != 0 ) {
      GetNArray( layer_ff->narr_weights, narr );
      t = narr->total;
      for ( j = 0; j < t; j++ ) {
        layer_ff->weights[j] *= m;
      }
    }
  }

  return self;
}

/* @overload run( input )
 * Runs network forward and generates a result
 * @param [NArray<sfloat>] input single input vector
 * @return [NArray<sfloat>] output of network
 */
VALUE network_rbobject__run( VALUE self, VALUE rv_input ) {
  Network *network = get_network_struct( self );
  Layer_FF *layer_ff;
  int i;
  int out_shape[1] = { network->num_outputs };

  struct NARRAY *na_input;
  volatile VALUE val_input = na_cast_object(rv_input, NA_SFLOAT);
  GetNArray( val_input, na_input );

  // Shouldn't happen, but we don't want a segfault
  if ( network->num_layers < 1 ) {
    return Qnil;
  }

  if ( na_input->total != network->num_inputs ) {
    rb_raise( rb_eArgError, "Input array must be size %d, but it was size %d", network->num_inputs, na_input->total );
  }

  struct NARRAY *na_output;

  volatile VALUE val_output = na_make_object( NA_SFLOAT, 1, out_shape, cNArray );
  GetNArray( val_output, na_output );

  Data_Get_Struct( network->layers[0], Layer_FF, layer_ff );
  layer_ff__run( layer_ff, (float*) na_input->ptr, network->activations[0] );

  for ( i = 1; i < network->num_layers; i++ ) {
    // TODO: This only works for Layer_FF layers, we need a more flexible system
    Data_Get_Struct( network->layers[i], Layer_FF, layer_ff );
    layer_ff__run( layer_ff, network->activations[i-1], network->activations[i] );
  }

  memcpy( (float*) na_output->ptr, network->activations[network->num_layers-1], network->num_outputs * sizeof(float) );

  return val_output;
}


/* @overload activations( layer_id )
 * Array of activation values from last call to .run from layer identified by layer_id
 * @param [NArray<sfloat>] input single input vector
 * @return [NArray<sfloat>] output of network
 */
VALUE network_rbobject__activations( VALUE self, VALUE rv_layer_id ) {
  Network *network = get_network_struct( self );
  Layer_FF *layer_ff;
  int layer_id = NUM2INT( rv_layer_id );

  if ( layer_id < 0 || layer_id >= network->num_layers ) {
    return Qnil; // Should this raise instead? Not sure . . .
  }

  Data_Get_Struct( network->layers[ layer_id ], Layer_FF, layer_ff );
  int out_shape[1] = { layer_ff->num_outputs };

  struct NARRAY *na_output;

  volatile VALUE val_output = na_make_object( NA_SFLOAT, 1, out_shape, cNArray );
  GetNArray( val_output, na_output );

  memcpy( (float*) na_output->ptr, network->activations[layer_id], layer_ff->num_outputs * sizeof(float) );

  return val_output;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void init_network_class( ) {
  // Network instantiation and class methods
  rb_define_alloc_func( RuNeNe_Network, network_alloc );
  rb_define_method( RuNeNe_Network, "initialize", network_rbobject__initialize, 1 );
  rb_define_method( RuNeNe_Network, "initialize_copy", network_rbobject__initialize_copy, 1 );

  // Network attributes
  rb_define_method( RuNeNe_Network, "layers", network_rbobject__get_layers, 0 );
  rb_define_method( RuNeNe_Network, "num_layers", network_rbobject__get_num_layers, 0 );
  rb_define_method( RuNeNe_Network, "num_inputs", network_rbobject__get_num_inputs, 0 );
  rb_define_method( RuNeNe_Network, "num_outputs", network_rbobject__get_num_outputs, 0 );

  // Network methods
  rb_define_method( RuNeNe_Network, "init_weights", network_rbobject__init_weights, -1 );
  rb_define_method( RuNeNe_Network, "run", network_rbobject__run, 1 );
  rb_define_method( RuNeNe_Network, "activations", network_rbobject__activations, 1 );
}
