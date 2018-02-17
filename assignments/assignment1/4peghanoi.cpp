// Recursion in C++ illustration -- Tower of Hanoi Problem with 4 pegs
// Written by Prof. Sreenivas for IE523: Financial Computing
// Homework #3
#include <iostream>
#include <cmath>
using namespace std;

// recursive function that computes the required moves assuming we have 3 pegs
void Move_Using_Three_Pegs (int n, char source, char destination, char intermediate) 
{ 
	if (n > 0) { 
		Move_Using_Three_Pegs (n-1, source, intermediate, destination); 
		cout << "Move the top disk from peg " << source << " to peg " << destination << endl;
		Move_Using_Three_Pegs (n-1, intermediate, destination, source); 
	} 
	/* if n == 0, do nothing */ 
	return;
} 

// recursive function that computes the required moves using the Frame-Stewart method
void Move_Using_Four_Pegs (int n, char source, char destination, char intermediate_1, char intermediate_2)
{
	if (n > 0) {
		int k = floor(sqrt(2*n)); //This is the optimal value for k 
		Move_Using_Four_Pegs (n-k, source, intermediate_1, intermediate_2, destination);
        Move_Using_Four_Pegs (k-1, source, intermediate_2, destination, intermediate_1);
        cout << "Move the top disk from peg" << source << " to peg" << destination << endl;
        Move_Using_Four_Pegs (k-1, intermediate_2, destination, intermediate_1, source);
		Move_Using_Four_Pegs (n-k, intermediate_1, destination, intermediate_2, source);
	}
	/* if n == 0, do
     nothing */
	return;
}

int main () 
{ 
	int no_of_disks;
    for (no_of_disks = 3; no_of_disks <= 10; no_of_disks++){
//        cout << "Enter the #disks in peg A: ";
//        cin >> no_of_disks;
        cout << "no_of_disks = " << no_of_disks << endl;
        if (no_of_disks <= 10)
            Move_Using_Four_Pegs (no_of_disks, 'A', 'D', 'B', 'C');
        else {
            cout << "This is going to take too long... exiting" << endl;
            exit (0); 
        }
    }
	
} 




