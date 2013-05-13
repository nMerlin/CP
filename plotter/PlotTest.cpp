#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>

#include "plotter.h"

double testFkt (double x)
{
    double a = 1.;
    double b = 1.;
    return a*x*x + b*x;
}

//main
int main () {
	//int N = 100;
	std::string pfad = "./";
	
	plotter plt;
	plt.setxrange(0,6);
	
	plt.plot(&testFkt);
	return 0;
}