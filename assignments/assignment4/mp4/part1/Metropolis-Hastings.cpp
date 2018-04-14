//
//  main.cpp
//  Multivariate Gaussian via Metropolis-Hastings
//
//  IE531: Algorithms for Data Analytics mp4 part1
//
//  Zhenye Na Apr 13, 2018
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>

//Substitue the directory here with your own NEWMAT library path
#include "/Users/macbookpro/newmat11/include.h"
#include "/Users/macbookpro/newmat11/newmat.h"
#include "/Users/macbookpro/newmat11/newmatio.h"

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
    // RV "x" that has a mean of "mean" and a matrix that is the identity matrix
    for (int i = 1; i <= mean.nrows(); i++) {
        x(i) = get_gaussian(mean(i), 1);
    }
    return x;
}

double MH_Discriminant(ColumnVector Current_value, ColumnVector Previous_value, SymmetricMatrix C, ColumnVector mean)
{
    // Write some C++ code here that computes equation 2 of the assignment description
    // double x_hat = get_gaussian(mean(mean.nrows()), 1);
    
    Matrix c = (Current_value - mean).t() * C.i() * (Current_value - mean);
    double up_c = c(1, 1);
    double up = exp(-0.5 * up_c);
    
    Matrix p = (Previous_value - mean).t() * C.i() * (Previous_value - mean);
    double down_p = p(1, 1);
    double down = exp(-0.5 * down_p);
    
    double re = up/down;
    
    if (re < 1) {
        return re;
    }
    else {
        return 1.0;
    }
}

double Theoretical_PDF(ColumnVector x, SymmetricMatrix C, ColumnVector mean)
{
    // write C++ code that computes the expression of equation 1 of the assignment description
    double multi_pdf;
    int d = mean.nrows();
    
    // Determinant of C
    double det = C.determinant();
    
    // Multipliers
    double multi = pow ((2 * PI), d);
    
    // Multivariate Gaussian Distribution
    Matrix m = -0.5 * (x - mean).t() * C.i() * (x - mean);
    double mm = m(1, 1);
    
    multi_pdf = (1 / pow (det * multi, 0.5)) * exp (mm);
    return multi_pdf;
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
