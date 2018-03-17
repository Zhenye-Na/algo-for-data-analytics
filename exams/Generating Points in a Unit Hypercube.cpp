//
//  main.cpp
//  Generating Points in a Unit Hypercube
//
//  Created by Ramavarapu Sreenivas on 3/16/18.
//  Copyright © 2018 Ramavarapu Sreenivas. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include "include.h"
#include "newmat.h"

using namespace std;

// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);

double get_uniform()
{
    // http://www.cplusplus.com/reference/random/uniform_int_distribution/
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}

int get_uniform_int(int low_value, int high_value)
{
    // http://www.cplusplus.com/reference/random/uniform_int_distribution/
    std::uniform_int_distribution<int> distribution(low_value, high_value);
    int number = distribution(generator);
    return number;
}

ColumnVector UIID_over_Unit_Hypercube_Surface(int dimension)
{
    // randomly pick a dimension to "freeze" at either 0 or 1
    int frozen_dimension = get_uniform_int(1, dimension);
    // randomly decide if the frozen-dimension is to be frozen at 0 or 1
    double frozen_dimension_value = (double) get_uniform_int(0,1);
    
    // assign the rest/unfrozen dimensions any value from get_uniform()
    ColumnVector x(dimension);
    for (int i = 1; i <= dimension; i++)
        x(i) = get_uniform();
    x(frozen_dimension) = frozen_dimension_value;
    
    return x;
}

ColumnVector UIID_over_Unit_Hypercube_Volume(int dimension)
{
    ColumnVector x(dimension);
    for (int i = 1; i <= dimension; i++)
        x(i) = get_uniform();
    
    return x;
}

int main (int argc, char* argv[])
{
    int no_of_trials, dimension;
    
    // 3D case
    dimension = 3;
    
    sscanf (argv[1], "%d", &no_of_trials);
    ofstream data1(argv[2]);
    ofstream data2(argv[3]);
    
    for (int i = 0; i < no_of_trials; i++)
    {
        ColumnVector x(dimension);
        x = UIID_over_Unit_Hypercube_Surface(dimension);
        data1 << x(1) << " " << x(2) << " " << x(3) << endl;
    }
    
    for (int i = 0; i < no_of_trials; i++)
    {
        ColumnVector x(dimension);
        x = UIID_over_Unit_Hypercube_Volume(dimension);
        data2 << x(1) << " " << x(2) << " " << x(3) << endl;
    }
    
}