require 'helpers'

describe RuNeNe::Network do
  let( :in_layer_xor ) { RuNeNe::Layer::FeedForward.new( 2, 2 ) }
  let( :out_layer_xor ) { RuNeNe::Layer::FeedForward.new( 2, 1 ) }

  describe "class methods" do
    describe "#new" do
      it "creates a new network" do
        expect( RuNeNe::Network.new( [in_layer_xor, out_layer_xor] ) ).to be_a RuNeNe::Network
      end

      it "refuses to create new networks for bad parameters" do
        expect { RuNeNe::Network.new( 17 ) }.to raise_error TypeError
        expect { RuNeNe::Network.new( [] ) }.to raise_error ArgumentError
        expect { RuNeNe::Network.new( [in_layer_xor,nil,out_layer_xor] ) }.to raise_error TypeError
      end
    end

    describe "with Marshal" do
      it "can save and retrieve a network, preserving layer properties" do
        orig_network = RuNeNe::Network.new( [in_layer_xor, out_layer_xor] )
        saved = Marshal.dump( orig_network )
        copy_network = Marshal.load( saved )

        expect( copy_network ).to_not be orig_network
        orig_layers = orig_network.layers
        copy_layers = copy_network.layers
        expect( copy_network.num_inputs ).to eql orig_network.num_inputs
        expect( copy_network.num_outputs ).to eql orig_network.num_outputs

        orig_layers.zip(copy_layers).each do |orig_layer, copy_layer|
          expect( copy_layer ).to_not be orig_layer
          expect( copy_layer.num_inputs ).to eql orig_layer.num_inputs
          expect( copy_layer.num_outputs ).to eql orig_layer.num_outputs
          expect( copy_layer.transfer ).to be RuNeNe::Transfer::Sigmoid
          expect( copy_layer.weights ).to be_narray_like orig_layer.weights
        end
      end
    end
  end

  describe "instance methods" do
    before :each do
      @nn = RuNeNe::Network.new( [in_layer_xor, out_layer_xor] )
    end

    describe "clone" do
      it "should make a simple copy of number of inputs and outputs" do
        copy = @nn.clone
        expect( copy.num_inputs ).to eql @nn.num_inputs
        expect( copy.num_outputs ).to eql @nn.num_outputs
      end

      it "should make a deep copy of layers" do
        copy = @nn.clone
        expect( copy.num_inputs ).to eql @nn.num_inputs
        expect( copy.num_outputs ).to eql @nn.num_outputs

        orig_layers = @nn.layers
        copy_layers = copy.layers

        orig_layers.zip(copy_layers).each do |orig_layer, copy_layer|
          expect( copy_layer ).to_not be orig_layer
          expect( copy_layer.num_inputs ).to eql orig_layer.num_inputs
          expect( copy_layer.num_outputs ).to eql orig_layer.num_outputs
          expect( copy_layer.transfer ).to be RuNeNe::Transfer::Sigmoid
          expect( copy_layer.weights ).to be_narray_like orig_layer.weights
        end
      end
    end
  end
end