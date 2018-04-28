//
//  main.cpp
//  Matrix Sketch
//
//  Created by Ramavarapu Sreenivas on 4/27/2018.
//  Copyright © 2017 Zhenye Na. All rights reserved.
//
//  Edo Liberty's paper is about using a streaming algorithm for
//  Matrix Sketching.  But we are going to implement/test this in
//  in a non-streaming context.  "A" is a large (n x m) data-matrix
//  where m >>> n. We want to find a smaller (n x k) "B" matrix, where
//  k = 2/epsilon, such that the Frobenius-Norm of (A*A.t() - B*B.t())
//  is <= epsilon * the Frobenius-Norm of (A*A.t())

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>

// Substitue the directory here with your own NEWMAT library path
#include "/Users/macbookpro/newmat11/include.h"
#include "/Users/macbookpro/newmat11/newmat.h"
#include "/Users/macbookpro/newmat11/newmatio.h"
#include "/Users/macbookpro/newmat11/newmatap.h"

using namespace std;

double Frobenious_Norm(Matrix Data)
{
    double norm = 0.0;
    
    // Compute AA^T
    Matrix T = Data * Data.t();
    
    // Shape of T
    int m = T.nrows();
    int n = T.ncols();
    
    // Compute Frobenious Norm
    for(int i = 1; i <= m; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            norm += T(i,j) * T(i,j);
        }
    }
    return sqrt(norm);
    
}

Matrix Matrix_Sketch(Matrix Data, double epsilon)
{
    // Edo Liberty's Matrix Sketch will have the same number of rows
    // as the original data; the #cols is ceil(2.0/epsilon)
    int cols_of_sketch = ceil(2.0/epsilon);
    
    if (cols_of_sketch < Data.nrows())
    {
        Matrix Result(Data.nrows(), cols_of_sketch);
        Matrix D_(cols_of_sketch, cols_of_sketch);
        ColumnVector C(Data.nrows());
        
        DiagonalMatrix D;
        Matrix U, V;
        
        
        Result = 0.0;
        D_ = 0.0;
        C = 0.0;
        
        for (int i = 1; i <= Data.Ncols(); i++) {
            for (int j = 1; j <= Result.Ncols(); j++) {

                if (Result.Column(j) == C) {
                    Result.Column(j) = Data.Column(i);
                    break;
                }

                else {
                    SVD(Result, D, U, V);
                    double delta = D(cols_of_sketch) * D(cols_of_sketch);
                    IdentityMatrix I(D.Nrows());
                    D = (D * D.t() - delta * I);
                    
                    for (int k = 1; k <= cols_of_sketch; k++) {
                        D_(k, k) = sqrt(D(k, k));
                    }
                    
                    Result = U * D_;
                }
            }
        }
        
        return Result;
    }
    else
    {
        Matrix Result(Data.nrows(), Data.nrows());
        Matrix D_(Data.nrows(), Data.nrows());

        DiagonalMatrix D;
        Matrix U, V;
        
        Result = 0.0;
        D_ = 0.0;
        
        // Perform SVD on matrix
        SVD(Data * Data.t(), D, U, V);
        
        for (int i = 1; i <= Data.Nrows(); i++) {
            D_(i, i) = sqrt(D(i, i));
        }
        
        Result = U * D_ * V.t();
        return Result;
    }
    
}

int main (int argc, char* argv[])
{
    int dimension, no_of_data_points;
    double epsilon;
    
    sscanf (argv[1], "%d", &dimension);
    sscanf (argv[2], "%d", &no_of_data_points);
    sscanf (argv[3], "%lf", &epsilon);
    ifstream input_file(argv[4]);
    ofstream output_file(argv[5]);
    
    Matrix Data(dimension, no_of_data_points);
    
    cout << "Edo Liberty's Matrix Sketching Algorithm" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Original Data-Matrix has " << dimension << "-rows & " << no_of_data_points << "-cols" << endl;
    cout << "Epsilon = " << epsilon << " (i.e. max. of " << 100*epsilon << "% reduction of Frobenius-Norm of the Sketch Matrix)"<< endl;
    cout << "Input File = " << argv[4] << endl;
    
    // Read the Data (Data.t())
    for (int i = 1; i <= dimension; i++)
        for (int j = 1; j <= no_of_data_points; j++)
        {
            double x;
            input_file >> x;
            Data(i,j) = x;
        }
    
    // Compute the Frobenius-Norm of the original Data-Matrix
    double Data_Forbenius_Norm = Frobenious_Norm(Data);
    cout << "Frobenius Norm of the (" << Data.nrows() << " x " << Data.ncols() << ") Data Matrix = ";
    cout << Data_Forbenius_Norm << endl;
    
    Matrix Sketch(dimension, min(dimension, (int) ceil(2/epsilon)));
    Sketch = Matrix_Sketch(Data, epsilon);
    double Sketch_Forbenius_Norm = Frobenious_Norm(Sketch);
    cout << "Frobenius Norm of the (" << Sketch.nrows() << " x " << Sketch.ncols() << ") Sketch Matrix = ";
    cout << Sketch_Forbenius_Norm << endl;
    cout << "Change in Frobenius-Norm between Sketch & Original  = ";
    cout << setprecision(3) << 100*(Sketch_Forbenius_Norm - Data_Forbenius_Norm)/Data_Forbenius_Norm << "%" << endl;
    
    output_file << Sketch;
    cout << "File `" << argv[5] << "' contains a (" << Sketch.nrows() << " x " << Sketch.ncols();
    cout << ") Matrix-Sketch" << endl;
    
    
}
