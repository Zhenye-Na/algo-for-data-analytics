//
//  main.cpp
//  Multivariate Gaussian via Metropolis-Hastings
//
//  Created by Ramavarapu Sreenivas on 3/10/17.
//  Copyright © 2017 Ramavarapu Sreenivas. All rights reserved.
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
#include "newmatio.h"

#define PI 3.141592654

using namespace std;

// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);

double get_gaussian(double mean, double standard_deviation)
{
    std::normal_distribution<double> distribution(mean, standard_deviation);
    double number = distribution(generator);
    return (number);
}

double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}

ColumnVector Generate_Independent_Multivariate_Gaussian(ColumnVector mean)
{
    ColumnVector x(mean.nrows());
    // Write some C++ code here that will generate "x" that has a Multivariate Gaussian
    //  RV "x" that has a mean of "mean" and a matrix that is the identity matrix
    return x;
}

double MH_Discriminant(ColumnVector Current_value, ColumnVector Previous_value, SymmetricMatrix C, ColumnVector mean)
{
    // Write some C++ code here that computes equation 2 of the assignment description
    return (x);
}

double Theoretical_PDF(ColumnVector x, SymmetricMatrix C, ColumnVector mean)
{
    // write C++ code that computes the expression of equation 1 of the assignment description
}

int main (int argc, char* argv[])
{
    ColumnVector y_prev, y_current;
    Matrix count(100,100);
    int no_of_trials, dimension;
    
    // 2D case
    dimension = 2;
    
    sscanf (argv[1], "%d", &no_of_trials);
    ofstream pdf_data(argv[2]);
    ofstream pdf_theory(argv[3]);
    
    // The covariance matrix
    SymmetricMatrix C(2);
    C(1,1) = 1.0;
    C(1,2) = 0.5;
    C(2,1) = 0.5;
    C(2,2) = 1.0;
    
    // The mean vector
    ColumnVector mean(2);
    mean(1) = 1.0;
    mean(2) = 2.0;
    
    cout << "Multivariate Gaussian Generator using MCMC-MH" << endl;
    cout << "Dimension = " << mean.nrows() << endl;
    cout << endl << "Mean Vector = " << endl << mean;
    cout << endl << "Covariance Matrix = " << endl << C;
    
    for (int i = 1; i <= 100; i++)
        for (int j = 1; j <= 100; j++)
            count(i,j) = 0.0;
    
    y_prev = Generate_Independent_Multivariate_Gaussian(mean);
    for (int i = 0; i < no_of_trials; i++)
    {
        y_current = Generate_Independent_Multivariate_Gaussian(mean);
        
        if (get_uniform() < MH_Discriminant(y_current, y_prev, C, mean))
        {
            for (int j = 1; j <= 100; j++) {
                for (int k = 1; k <= 100; k++) {
                    if ( (y_current(1) >= ((double) (j-52)/10)) && (y_current(1) < ((double) (j-51)/10)) &&
                        (y_current(2) >= ((double) (k-52)/10)) && (y_current(2) < ((double) (k-51)/10)) )
                        count(j,k)++;
                }
            }
            y_prev = y_current;
        }
    }
    
    for (int j = 1; j <= 100; j++) {
        for (int k = 1; k <= 100; k++) {
            if (k < 100)
                pdf_data << count(j,k)/((double) no_of_trials) << ", ";
            if (k == 100)
                pdf_data << count(j,k)/((double) no_of_trials) << endl;
        }
    }
    
    double x1, x2;
    for (int j = 1; j <= 100; j++) {
        x1 = ((double) (j-51)/10);
        for (int k = 1; k <= 100; k++) {
            x2 = ((double) (k-51)/10);
            ColumnVector x(2);
            x(1) = x1;
            x(2) = x2;
            if (k < 100)
                pdf_theory << Theoretical_PDF(x, C, mean)*0.01 << ", ";
            if (k == 100)
                pdf_theory << Theoretical_PDF(x, C, mean)*0.01 << endl;
        }
    }
}
