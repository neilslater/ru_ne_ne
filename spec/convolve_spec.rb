require 'helpers'

describe RuNeNe do
  describe "#convolve" do

    it "should work like the example in the README" do
      a = NArray[ 0.3, 0.4, 0.5 ]
      b = NArray[ 1.3, -0.5 ]
      c = RuNeNe.convolve( a, b )
      expect( c ).to be_narray_like NArray[ 0.19, 0.27 ]
    end

    it "should calculate a 2D convolution" do
      a = NArray[ [ 0.3, 0.4, 0.5 ], [ 0.6, 0.8, 0.2 ], [ 0.9, 1.0, 0.1 ] ]
      b = NArray[ [ 1.2, -0.5 ], [ 0.5, -1.3 ] ]
      c = RuNeNe.convolve( a, b )
      expect( c ).to be_narray_like NArray[ [ -0.58, 0.37 ], [ -0.53, 1.23 ] ]
    end

    it "should calculate a 2D convolution with rectangular arrays" do
      a = NArray[ [ 0.3, 0.4, 0.5, 0.3, 0.4 ], [ 0.6, 0.8, 0.2, 0.8, 0.2 ],
        [ 0.9, 1.0, 0.1, 0.9, 1.0 ], [ 0.5, 0.9, 0.3, 0.2, 0.8 ], [ 0.7, 0.1, 0.3, 0.0, 0.1 ],
        [ 0.4, 0.5, 0.6, 0.7, 0.8 ], [ 0.5, 0.4, 0.3, 0.2, 0.1 ] ]
      b = NArray[ [ 1.2, -0.5, 0.2 ], [ 1.8, 0.5, -1.3 ] ]
      c = RuNeNe.convolve( a, b )
      expect( c ).to be_narray_like NArray[ [ 1.48, 0.79, 1.03 ], [ 2.35, 1.7, -0.79 ], [ 1.56, 2.84, -0.53 ],
        [ 1.13, 1.3, 0.83 ], [ 1.04, 0.26, 0.77 ], [ 1.06, 1.05, 1.04 ] ]
    end

    it "should calculate a 3D convolution" do
      # 5x4x3
      a = NArray[
        [ [ 1.0, 0.6, 1.1, 0.2, 0.9 ], [ 1.0, 0.7, 0.8, 1.0, 1.0 ], [ 0.2, 0.6, 0.1, 0.2, 0.5 ], [ 0.5, 0.9, 0.2, 0.1, 0.6 ] ],
        [ [ 0.4, 0.9, 0.4, 0.0, 0.6 ], [ 0.2, 1.1, 0.2, 0.4, 0.1 ], [ 0.4, 0.2, 0.5, 0.8, 0.7 ], [ 0.1, 0.9, 0.7, 0.1, 0.3 ] ],
        [ [ 0.8, 0.6, 1.0, 0.1, 0.4 ], [ 0.3, 0.8, 0.6, 0.7, 1.1 ], [ 0.9, 1.0, 0.3, 0.4, 0.6 ], [ 0.2, 0.5, 0.4, 0.7, 0.2 ] ]
      ]

      # 3x3x3
      b = NArray[
        [ [ -0.9, 1.2, 0.8  ], [ 0.9, 0.1, -0.5 ], [ 1.1, 0.1, -1.1 ] ],
        [ [ -0.2, -1.0, 1.4 ], [ -1.4, 0.0, 1.3 ], [ 0.3, 1.0, -0.5 ] ],
        [ [ 0.6, 0.0, 0.7 ],   [ -0.7, 1.1, 1.2 ], [ 1.3, 0.7, 0.0  ] ]
      ]

      # Should be 3x2x1
      c = RuNeNe.convolve( a, b )
      expect( c ).to be_narray_like NArray[ [ [ 5.51, 3.04, 4.3 ], [ 3.04, 6.31, 3.87 ] ] ]
    end

    it "should calculate a 4D convolution" do
      # 3x4x5x3
      a = NArray[
        [ [ [ 0.5, 0.4, 0.9 ], [ 0.1, 0.9, 0.8 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ] ],
        [ [ [ 0.5, 0.4, 0.9 ], [ 0.1, 0.9, 0.8 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ] ],
        [ [ [ 0.5, 0.4, 0.9 ], [ 0.1, 0.9, 0.8 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ],
          [ [ 0.0, 0.4, 0.0 ], [ 0.2, 0.3, 0.8 ], [ 0.6, 0.3, 0.2 ], [ 0.7, 0.4, 0.3 ] ],
          [ [ 0.3, 0.3, 0.1 ], [ 0.6, 0.9, 0.4 ], [ 0.4, 0.0, 0.1 ], [ 0.8, 0.3, 0.4 ] ] ] ]

      # 2x3x3x2
      b = NArray[ [
        [ [ 1.1, 0.6 ], [ 1.2, 0.6 ], [ 0.8, 0.1 ] ], [ [ -0.4, 0.8 ], [ 0.5, 0.4 ], [ 1.2, 0.2 ] ],
        [ [ 0.8, 0.2 ], [ 0.5, 0.0 ], [ 1.4, 1.3 ] ] ],
        [ [ [ 1.1, 0.6 ], [ 1.2, 0.6 ], [ 0.8, 0.1 ] ], [ [ -0.4, 0.8 ], [ 0.5, 0.4 ], [ 1.2, 0.2 ] ],
        [ [ 0.8, 0.2 ], [ 0.5, 0.0 ], [ 1.4, 1.3 ] ] ] ]

      # Should be 2x2x3x2
      c = RuNeNe.convolve( a, b )
      expect( c ).to be_narray_like NArray[
        [ [ [ 8.5, 8.2 ], [ 11.34, 9.68 ] ], [ [ 7.68, 6.56 ], [ 11.24, 7.16 ] ], [ [ 9.14, 6.54 ], [ 12.44, 9.2 ] ] ],
        [ [ [ 8.5, 8.2 ], [ 11.34, 9.68 ] ], [ [ 7.68, 6.56 ], [ 11.24, 7.16 ] ], [ [ 9.14, 6.54 ], [ 12.44, 9.2 ] ] ]
      ]
    end
  end
end
