//
//  main.cpp
//  Johnson_Lindenstrauss
//
//  Created by Ramavarapu Sreenivas on 4/3/17.
//  Copyright Â© 2017 Ramavarapu Sreenivas. All rights reserved.
//


#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <chrono>
#include <string.h>
#include "/Users/macbookpro/newmat11/include.h"
#include "/Users/macbookpro/newmat11/newmat.h"
#include "/Users/macbookpro/newmat11/newmatio.h"
#include "/Users/macbookpro/newmat11/newmatap.h"

typedef std::pair<double, double> Box_Muller_Pair;

using namespace std;


// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

// U.I.I.D. RV generator
double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0, 1.0);
    double number = distribution(generator);
    return (number);
}

// Using the Gaussian generator that is part of the C++ STL
double get_gaussian(double mean, double standard_deviation)
{
    std::normal_distribution<double> distribution(mean, standard_deviation);
    double number = distribution(generator);
    return (number);
}

Matrix Generate_Random_Projection(int k, int d)
{
    // write code here that follows what we discussed in class
    Matrix R(k, d);
    for (int j = 1; j <= k; j++)
    {
        for (int i = 1; i <= d; i++)
        {
            double x = get_gaussian(0, 1 / sqrt(k));
            R(j, i) = x;
        }
    }
    return R;
}

int main(int argc, const char * argv[])
{
    int original_dimension, reduced_dimension, no_of_cols, no_of_trials;
    double epsilon, delta, diff;
    clock_t time_before, time_after;
    
    sscanf(argv[1], "%d", &original_dimension);
    sscanf(argv[2], "%d", &reduced_dimension);
    sscanf(argv[3], "%d", &no_of_cols);
    sscanf(argv[4], "%lf", &epsilon);
    sscanf(argv[5], "%lf", &delta);
    ifstream input_file(argv[6]);
    sscanf(argv[7], "%d", &no_of_trials);
    
    
    cout << "Johnson-Lindenstrauss Lemma Demo" << endl;
    cout << "Reading a (" << original_dimension << " x " << no_of_cols << ") Matrix from file '";
    cout << argv[6] << "'" << endl;
    cout << "Reduced Dimension = " << reduced_dimension << endl;
    cout << "epsilon = " << epsilon << endl;
    cout << "delta = " << delta << endl;
    cout << "Reduced Dimension (i.e. " << reduced_dimension << ") should be >= " << ceil(1 / (epsilon*epsilon)*log(((double)no_of_cols) / delta));
    cout << " for the bound to hold with probability " << 1 - delta << endl;
    
    Matrix R = Generate_Random_Projection(reduced_dimension, original_dimension);
    
    Matrix A(original_dimension, no_of_cols);
    
    
    
    time_before = clock(); // recording time before we started to read data
    for (int j = 1; j <= original_dimension; j++)
    {
        for (int i = 1; i <= no_of_cols; i++)
        {
            double x;
            input_file >> x;
            A(j, i) = x;
        }
    }
    time_after = clock(); // recording time after testing is complete
    diff = ((double)time_after - (double)time_before);
    cout << "It took " << (diff / CLOCKS_PER_SEC) / 60.0 << " minutes to read data from file '" << argv[6] << "'" << endl;
    
    // testing Johnson-Lindenstrauss Lemma
    int no_of_hits = 0;
    cout << "#Trails for the testing-phase = " << no_of_trials << endl;
    // this is the reduced-dimension representation of the x's (i.e the matrix of y's)
    Matrix C(reduced_dimension, original_dimension);
    C = R * A;
    
    time_before = clock(); // recording time before the testing starts
    
    // write code here for verification of JL-Lemma
    for (int num = 1; num <= no_of_trials; num++) {
        // Select two examples randomly, but not the same two index
        int idx1, idx2;
        
        do {
            idx1 = get_uniform() * no_of_cols;
            idx2 = get_uniform() * no_of_cols;
        } while (idx1 == idx2);
        
        
        double diff_y = 0;
        for (int row = 1; row <= reduced_dimension; row++) {
            diff_y += pow(C(row, idx1) - C(row, idx2), 2);
        }
        double norm_y = sqrt(diff_y);
        
        
        
        double diff_x = 0;
        for (int row = 1; row <= original_dimension; row++) {
            diff_x += pow(A(row, idx1) - A(row, idx2), 2);
        }
        double norm_x = sqrt(diff_x);
        
        
        
        if ((norm_y <= (1 + epsilon) * norm_x) && (norm_y >= (1 - epsilon) * norm_x)) {
            no_of_hits++;
        }
        
        
    }
    
    
    time_after = clock(); // recording time after testing is complete
    diff = ((double)time_after - (double)time_before);
    cout << "It took " << (diff / CLOCKS_PER_SEC) / 60.0 << " minutes for testing to be completed" << endl;
    
    cout << "Johnson-Lindenstrauss Lemma is satisfied " << no_of_hits << "-many times over ";
    cout << no_of_trials << " attempts" << endl;
    cout << "Empirical Probability = " << ((double)no_of_hits / no_of_trials);
    
    return 0;
}