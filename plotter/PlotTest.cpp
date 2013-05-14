#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>

#include "plotter.h"

double testFkt1 (double x)
{
    double a = 1.;
    double b = 1.;
    return a*x*x + b*x;
}
double testFkt2 (double x)
{
    double a = 0.5;
    double b = 1.;
    return a*x*x + b*x;
}
double testFkt3 (double x)
{
    double a = 1.;
    double b = 2.;
    return a*x*x + b*x;
}

//main
int main () {
	//int N = 100;
	std::string pfad = "./";
	
	plotter plt;
	plt.setxrange(0,6);
	
// 	plt.plot(&testFkt1, pfad, "name1");
// 	plt.plot(&testFkt2, pfad, "name2");
// 	plt.plot(&testFkt3, pfad, "name3");
	
	plt.addDat("./name1.dat");
	plt.addDat("./name2.dat");
	plt.addDat("./name3.dat");
	plt.plotDat();
	
	
	return 0;
}