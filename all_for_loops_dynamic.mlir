#map=affine_map<(d0) -> (d0)>

builtin.module {
	"func.func"() ({ 
	^bb0(%in: memref<100xf32>, %out: memref<100xf32>):
		// Inter stage buffers
		%out_A = "memref.alloc"() {"operand_segment_sizes" = array<i32: 0, 0>} : () -> memref<100xf32>

		%out_B = "memref.alloc"() {"operand_segment_sizes" = array<i32: 0, 0>} : () -> memref<100xf32>

		%out_C = "memref.alloc"() {"operand_segment_sizes" = array<i32: 0, 0>} : () -> memref<100xf32>

		%out_D = "memref.alloc"() {"operand_segment_sizes" = array<i32: 0, 0>} : () -> memref<100xf32>

		%N_A = arith.constant 100 : index

		// A
		"affine.for"(%N_A) ({
			^bb0(%arg0 : index):
				%in_A = "affine.load"(%in, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				%0 = arith.constant 1234.0 : f32
				"affine.store"(%in_A, %out_A, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (f32, memref<100xf32>, index) -> ()
				"affine.yield"() : () -> ()
		}) {"lower_bound" = affine_map<() -> (0)>, "upper_bound" = #map, "step" = 1 : index} : (index) -> ()

		%N_B = arith.constant 100 : index
		// B
		"affine.for"(%N_B) ({
			^bb0(%arg0 : index):
				%1 = "affine.load"(%out_A, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				%2 = arith.constant 0.0 : f32
				"affine.store"(%1, %out_B, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (f32, memref<100xf32>, index) -> ()
				"affine.yield"() : () -> ()
		}) {"lower_bound" = affine_map<() -> (0)>, "upper_bound" = #map, "step" = 1 : index} : (index) -> ()

		%N_C = arith.constant 100 : index
		// C
		"affine.for"(%N_C) ({
			^bb0(%arg0 : index):
				%3 = "affine.load"(%out_B, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				%4 = arith.constant 0.0 : f32
				"affine.store"(%3, %out_C, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (f32, memref<100xf32>, index) -> ()
				"affine.yield"() : () -> ()
		}) {"lower_bound" = affine_map<() -> (0)>, "upper_bound" = #map, "step" = 1 : index} : (index) -> ()

		%N_D = arith.constant 100 : index
		// D
		"affine.for"(%N_D) ({
			^bb0(%arg0 : index):
				%5 = "affine.load"(%out_A, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				%6 = arith.constant 0.0 : f32
				"affine.store"(%6, %out_D, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (f32, memref<100xf32>, index) -> ()
				"affine.yield"() : () -> ()
		}) {"lower_bound" = affine_map<() -> (0)>, "upper_bound" = #map, "step" = 1 : index} : (index) -> ()

		%N_E = arith.constant 100 : index
		// E
		"affine.for"(%N_E) ({
			^bb0(%arg0 : index):
				%in_C = "affine.load"(%out_C, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				%in_D = "affine.load"(%out_D, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (memref<100xf32>, index) -> f32	
				"affine.store"(%in_C, %out, %arg0) {"map" = affine_map<(d0) -> (d0)>} : (f32, memref<100xf32>, index) -> ()
				"affine.yield"() : () -> ()
		}) {"lower_bound" = affine_map<() -> (0)>, "upper_bound" = #map, "step" = 1 : index} : (index) -> ()
		"func.return"() : () -> ()
	}) {"function_type" = (memref<100xf32>, memref<100xf32>) -> (), "sym_name" = "forward"} : () -> ()
}
