$ time ./a.out 50 1000 0.09 matrix_sketch1 matrix_sketch1_out
Edo Liberty's Matrix Sketching Algorithm
----------------------------------------
Original Data-Matrix has 50-rows & 1000-cols
Epsilon = 0.09 (i.e. max. of 9% reduction of Frobenius-Norm of the Sketch Matrix)
Input File = matrix_sketch1
Frobenius Norm of the (50 x 1000) Data Matrix = 12569.8
Frobenius Norm of the (50 x 23) Sketch Matrix = 12374.6
Change in Frobenius-Norm between Sketch & Original  = -1.55%
File `matrix_sketch1_out' contains a (50 x 23) Matrix-Sketch

real	0m5.097s
user	0m4.956s
sys	0m0.026s

$ time ./a.out 50 1000 0.1 matrix_sketch1 matrix_sketch1_out1
Edo Liberty's Matrix Sketching Algorithm
----------------------------------------
Original Data-Matrix has 50-rows & 1000-cols
Epsilon = 0.1 (i.e. max. of 10% reduction of Frobenius-Norm of the Sketch Matrix)
Input File = matrix_sketch1
Frobenius Norm of the (50 x 1000) Data Matrix = 12569.8
Frobenius Norm of the (50 x 20) Sketch Matrix = 12345.6
Change in Frobenius-Norm between Sketch & Original  = -1.78%
File `matrix_sketch1_out1' contains a (50 x 20) Matrix-Sketch

real	0m3.378s
user	0m3.241s
sys	0m0.020s

$ time ./a.out 9 329 0.2 Data x
Edo Liberty's Matrix Sketching Algorithm
----------------------------------------
Original Data-Matrix has 9-rows & 329-cols
Epsilon = 0.2 (i.e. max. of 20% reduction of Frobenius-Norm of the Sketch Matrix)
Input File = Data
Frobenius Norm of the (9 x 329) Data Matrix = 32803.6
Frobenius Norm of the (9 x 9) Sketch Matrix = 32803.6
Change in Frobenius-Norm between Sketch & Original  = 2.22e-14%
File `x' contains a (9 x 9) Matrix-Sketch

real	0m0.019s
user	0m0.006s
sys	0m0.003s
