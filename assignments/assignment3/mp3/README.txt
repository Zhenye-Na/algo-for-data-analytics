macbookpro ie531-project $ g++ main.cpp /path_to_/libnewmat.a
macbookpro ie531-project $ time ./a.out 5 400  r.txt b.txt
Computing Matrix Multiplication using Repeated Squaring Algorithm and Brute Force...
Number of rows                                  = 5
Number of exponents to be computed              = 400
Output filename of Repeated Squaring Algorithm  = r.txt
Output filename of Brute Force                  = b.txt

Initial Matrix: 
---------------------------------------------------------------------
 -4.99992	-3.68462	2.55605		-0.413498	0.327672 
 -2.81041	-4.52955	1.78865		1.79296		4.34693 
 -1.16498	0.194164	3.30965		-4.65428	-4.46538 
 0.297002	1.71149		-4.92302	-1.16584	-4.33158 
 -0.82514	1.86773		0.889766	4.30437		3.46167 
---------------------------------------------------------------------
Repeated Squaring Algorithm started...
It took 4.3e-05 second(s) to complete Matrix Multiplication using Repeated Squaring Algorithm.
Brute Force Algorithm started...
It took 0.000273 second(s) to complete Matrix Multiplication using Brute Force Algorithm.

real    0m0.074s
user    0m0.051s
sys 0m0.006s
