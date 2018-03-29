//
//  main.cpp
//  Computing Matrix Multiplication using Repeated Squaring Algorithm
//
//  Created by Zhenye Na on Mar 28, 2018.
//  Copyright © 2018 Zhenye Na. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <ctime>
#include <time.h>

//Substitue the directory here with your own NEWMAT library path
#include "/Users/macbookpro/newmat11/include.h"
#include "/Users/macbookpro/newmat11/newmat.h"
#include "/Users/macbookpro/newmat11/newmatio.h"
#include "/Users/macbookpro/newmat11/newmatap.h"

using namespace std;



// The following routine returns a uniformly distributed random number in
// the unit-interval... we will cover more of how this works later...
float get_uniform() {
    int low = -5;
    int high = 5;
    float rand_num = (float) ((high - low) * ((float) rand() / RAND_MAX) + low);
    return rand_num;
}



// This function fills a (n, n) matrix, with random entries

Matrix create_random_matrix(Matrix matrix, int d)
{
    matrix(d, d);
    for (int i = 1; i <= d; i++) {
        for(int j = 1; j<= d; j++) {
            matrix(i, j) = get_uniform();
        }
    }
    
    return(matrix);
}



// Compute matrix multiplication using Repeated Squaring Algorithm
// d -> no_of_rows
// k -> exponent
Matrix repeated_squaring(Matrix B, int exponent, int n) {
    B(n, n);
    IdentityMatrix I(n);
    
    if (exponent == 0) {
        return I;
    }
    if (exponent == 1) {
        return B;
    }
    if ( exponent % 2 == 1) {
        return (B * repeated_squaring(B*B, (exponent - 1)/2, n));
    } else {
        return (repeated_squaring(B*B, exponent / 2, n));
    }
    
}



// brute force matrix multiplication
Matrix brute_force_mul(Matrix B, int k, int n) {
    B(n, n);
    IdentityMatrix I(n);
    Matrix result(n, n);
    result = B;
    if (k == 0) {
        return I;
    }
    if (k == 1) {
        return B;
    }
    for (int i = 1; i < k; i++) {
        result = B * result;
    }
    return result;
}



// print matrix
void print_matrix(Matrix X, int n)
{
    X(n, n);
    cout << "-----------------------";
    
    for (int i = 1; i <= n; i++) {
        cout << endl;
        for (int j = 1; j <= n; j++) {
            cout << " " << X(i, j) << " ";
        }
    }
    cout << endl;
    cout << "-----------------------" << endl;
}



int main(int argc, char* argv[])
{
    
    int exponent;
    int dimension;
    
    sscanf(argv[1], "%d", &dimension);
    sscanf(argv[2], "%d", &exponent);
    ofstream outfile_1(argv[3]);
    ofstream outfile_2(argv[4]);
    
    cout << "Computing Matrix Multiplication using Repeated Squaring Algorithm and Brute Force..." << endl;
    cout << "Number of rows                                  = " << dimension << endl;
    cout << "Number of exponents to be computed              = " << exponent << endl;
    cout << "Output filename of Repeated Squaring Algorithm  = " << argv[3] << endl;
    cout << "Output filename of Brute Force                  = " << argv[4] << endl;
    cout << endl;
    
    Matrix B(dimension, dimension);
    Matrix C(dimension, dimension);
    Matrix D(dimension, dimension);
    Matrix temp(dimension, dimension);
    
    B = create_random_matrix(temp, dimension);
    
    cout<<"Initial Matrix: "<<endl;
    print_matrix(B, dimension);
    
    
    /* Repeated Squaring Algorithm */
    cout << "Repeated Squaring Algorithm started..." << endl;
    clock_t time_before1 = clock();
    
    C = repeated_squaring(B, exponent, dimension);
    
    clock_t time_after1 = clock();
    float diff1 = ((float) time_after1 - (float) time_before1);
    cout << "It took "<< diff1/CLOCKS_PER_SEC <<" second(s) to complete Matrix Multiplication using Repeated Squaring Algorithm."<< endl;
    //        cout<<"Repeated Squaring Result: "<<endl;
    //        print_matrix(C, dimension);
    
    
    
    /* Brute Force */
    cout << "Brute Force Algorithm started..." << endl;
    clock_t time_before2 = clock();
    
    D = brute_force_mul(B, exponent, dimension);
    
    clock_t time_after2 = clock();
    float diff2 = ((float) time_after2 - (float) time_before2);
    cout << "It took "<< diff2/CLOCKS_PER_SEC <<" second(s) to complete Matrix Multiplication using Brute Force Algorithm."<< endl;
    //        cout<<"Brute Force Result: "<<endl;
    //        print_matrix(D, dimension);
    
    double arr1[9999] = {};
    
    for (int iter = 1; iter <= exponent; iter++) {
        clock_t time_before1 = clock();
        
        C = repeated_squaring(B, iter, dimension);
        
        clock_t time_after1 = clock();
        float diff1 = ((float) time_after1 - (float) time_before1);
        //        cout << diff1/CLOCKS_PER_SEC << ", ";
        arr1[iter - 1] = diff1 / CLOCKS_PER_SEC;
        outfile_1 << arr1[iter - 1] << ", ";
        
    }
    
    
    
    double arr2[9999] = {};
    
    for (int iter = 1; iter <= exponent; iter++) {
        clock_t time_before2 = clock();
        
        D = brute_force_mul(B, iter, dimension);
        
        clock_t time_after2 = clock();
        float diff2 = ((float) time_after2 - (float) time_before2);
        //        cout << diff2/CLOCKS_PER_SEC << ", ";
        arr2[iter - 1] = diff2 / CLOCKS_PER_SEC;
        outfile_2 << arr2[iter - 1] << ", ";
        
    }
    
    return 0;
}
