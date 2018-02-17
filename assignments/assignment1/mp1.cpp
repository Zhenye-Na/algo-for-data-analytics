//
//  Generating Exponential and Normal RVs from an unknown discrete distribution
//
//  Created by Ramavarapu Sreenivas on 10/20/16.
//  Copyright Â© 2016 Ramavarapu Sreenivas. All rights reserved.
//
//  IE 531 2018 Spring mp1
//  Templated from Prof. Ramavarapu Sreenivas
//  Zhenye Na
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>

#define HEADS 1
#define TAILS 0
#define EVENT1 1
#define EVENT2 2
#define EVENT3 3
using namespace std;

double prob1, prob2, prob3;

// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
// unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
// default_random_engine generator (seed);
// initially just use default_random_engine generator;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);

// I need this uniform i.i.d. r.v. generator to simulate the unfair-coin in
// simulate_coin_toss(double heads_probability)
double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}

// Granted... the term "random-probabilities" is weird.  This is just a way
// to assign unpredictable-values for Prob(EVENT1), Prob(EVENT2) and Prob(EVENT3)
// with the proviso that Prob(EVENT1) + Prob(EVENT2) + Prob(EVENT3) = 1.
void assign_random_probabilities_to_three_events()
{
    // randomly assign values for prob1, prob2 and prob3 such that the 3-tuple
    // (prob1, prob2, prob3) is uniformly-distributed over the surface defined by
    // the equation prob1 + prob2 + prob3 = 1...
    // look at section 1.4 of the Prob. & Stats review for ways of generating
    // constrained RVs.
    
    // write code here
    prob1 = get_uniform();
    prob2 = get_uniform();
    if (prob1 + prob2 >= 1){
        assign_random_probabilities_to_three_events();
    }
    else{
        prob3 = 1 - (prob1 + prob2);
    }
}

// Discrete Probability Distribution Generator
// Generates EVENT1 with probability "prob1"
// Generates EVENT2 with probability "prob2"
// Generates EVENT3 with probability "prob3"
int three_event_generator(double prob1, double prob2, double prob3)
{
    // write code here

    double randint = get_uniform();
    if (randint <= prob1){
        return EVENT1;
    }
    else if (randint <= (prob2 + prob1)){
        return EVENT2;
    }
    else
        return EVENT3;
}


// This procedure converts the three-discerte-event-generator (where
// Prob(EVENT1) = prob1, Prob(EVENT2) = prob2, Prob(EVENT3) = 1-prob1-prob2
// into a fair-coin where Prob(HEADS) = Prob(TAILS) = 1/2.
int simulate_fair_coin_toss_from_three_event_generator(double prob1, double prob2, double prob3)
{
    // write code here
    
    // Initialize fair-coin model
    int e1 = three_event_generator(prob1, prob2, prob3);
    int e2 = three_event_generator(prob1, prob2, prob3);
    
    // Define fair-coin model
    if (e1 == EVENT1 && (e2 == EVENT2 || e2 == EVENT3)){
        return TAILS;
    }
    else if ((e1 == EVENT2 || e1 == EVENT3) && e2 == EVENT1){
        return HEADS;
    }
    else
        return (simulate_fair_coin_toss_from_three_event_generator(prob1, prob2, prob3));
}

// This procedure gets 32 fair-tosses from (> 32 tosses) of the unfair-coin
// For every "fair-heads" (resp. "fair-tails") we write down a "1" (resp. "0")
// and interpret the 32-bit unsigned integer as a random unsigned int between
// 0 and 2^32-1.
double get_uniform_from_three_event_generator(double prob1, double prob2, double prob3)
{
    // write code here
    
    // Define an array to record result of trials
    int trials [32];
    
    for (int i = 0; i < 32; i++) {
        trials[i] = simulate_fair_coin_toss_from_three_event_generator(prob1, prob2, prob3);
    }
    double sum = 0;
    for (int i = 0; i < 32; i++) {
        sum += trials[i] * pow(2, 31 - i);
    }
    return (sum / (pow(2, 32) - 1));
}

// This generates i.i.d. rv.s that are exponentially distributed with rate lambda
// using an unfair-coin
double get_exp_from_unfair_coin(double lambda, double prob1, double prob2)
{
    return ((-1.0/lambda)*log(get_uniform_from_three_event_generator(prob1, prob2, 1-prob1-prob2)));
}

// This generates i.i.d. rv.s that are unit-normal distributed using an unfair-coin
// (I am using the Box-Muller method here)
double get_gaussian_from_unfair_coin(double prob1, double prob2)
{
    return (sqrt(-2.0*log(get_uniform_from_three_event_generator(prob1, prob2, 1-prob1-prob2)))*cos(2*3.141592654*get_uniform_from_three_event_generator(prob1, prob2, 1-prob1-prob2)));
}

int main (int argc, char* argv[])
{
    
    double lambda;
    long no_of_trials;
    
    sscanf (argv[1], "%ld", &no_of_trials);
    sscanf (argv[2], "%lf", &lambda);
    ofstream outfile_1(argv[3]);
    ofstream outfile_2(argv[4]);
    
    assign_random_probabilities_to_three_events();
    
    cout << "Generating i.i.d. Unit-Normals and Exponentials from a 3-Event Generator" << endl;
    cout << "Probability of EVENT1                     = " << prob1 << endl;
    cout << "Probability of EVENT2                     = " << prob2 << endl;
    cout << "Probability of EVENT3                     = " << prob3 << endl;
    cout << "Number of trials                          = " << no_of_trials << endl;
    cout << "Output File Name for the Unit-Normal Data = " << argv[3] << endl;
    cout << "Output File Name for the Exponential Data = " << argv[4] << endl;
    cout << "Rate of Exponential RVs                   = " << lambda << endl;
    
    // i.i.d. exponential r.v. generation
    int count[100];
    for (int i = 0; i < 100; i++)
        count[i] = 0;
    
    for (int i = 0; i < no_of_trials; i++)
    {
        double y = get_exp_from_unfair_coin(lambda, prob1, prob2);
        for (int j = 0; j < 100; j++)
            if (y < ((double) j/10))
                count[j]++;
    }
    
    // Exponential CDF -- experimental vs. theory
    for (int j = 0; j < 100; j++)
        outfile_2 << ((double) j/10) << ", " << ((double) count[j])/((double) no_of_trials) << ", " << (1.0-exp(-1.0*lambda*((double) j/10))) << endl;
    
    // i.i.d. unit-normal generation
    double cdf_gaussian[100];
    for (int i = 0; i < 100; i++)
    {
        count[i] = 0;
        cdf_gaussian[i] = 0.0;
    }
    
    for (int i = 0; i < no_of_trials; i++)
    {
        double y = get_gaussian_from_unfair_coin(prob1, prob2);
        for (int j = 0; j < 100; j++)
            if (y < ((float) (j-50)/10))
                count[j]++;
    }
    // Unit-Normal CDF -- experimental vs. theory
    for (int j = 1; j < 100; j++)
        cdf_gaussian[j] = cdf_gaussian[j-1] +
        ((1/sqrt(2*3.141592654))*exp(-1.0*(((double) (j-50)/10)*
                                           ((double) (j-50)/10))/2)/10.0);
    
    for (int j = 0; j < 100; j++)
        outfile_1 << ((double) (j-50)/10) << ", " <<
        ((double) count[j])/((double) no_of_trials) << ", " << cdf_gaussian[j]
        << endl;
    
}