#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>

#include "A2_plot.h"

//Simpson-Regel zur Integration
double simpson (std::function<double(double)> function, double a, double b, double N /*N bitte gerade!*/) {
	
	//Variablen (intErg: Integrationsergebnis, h:Integrationsintervallbreite)
	double intErg = 0;
	double h = (b - a)/N;
	
	//Summe über die mit 1/3 gewichteten Randwerte
	intErg = intErg + h/3 * (function(a) + function(b));
	
	//Summe über die mit 4/3 und 2/3 gewichteten Summanden
	for (int i = 1; i < N; i++) {
		intErg = intErg + h * 2/3 * ((i % 2) + 1) * function(a + h * i);
	}
	
	return intErg;
}

//Integrand des Fraunhofer-Integrals
double integrand (double q, double r, double phi) {
		return -cos(q*r*cos(phi))*r;
}

//Berechnet Fraunhofer Beugungsintegral
double fraunhofer (double q, int N) {
	
	//***Variablen***
	double a = 1;
	double intErg = 0;
	double h = a/N;
	
	//Randwerte
	intErg = intErg + h/3 * (simpson(std::bind(&integrand,q,a,std::placeholders::_1),0,2*M_PI,N) + simpson(std::bind(&integrand,q,2*a,std::placeholders::_1),0,2*M_PI,N));
	
	//***Integration***
	//Für mehrdimensionale Integration wird zweimal die simpson-Regel angewendet
	//Summe über die mit 4/3 und 2/3 gewichteten Summanden
	for (int i = 1; i < N; i++) {
		intErg = intErg + h * 2/3 * ((i % 2) + 1) * simpson(std::bind(&integrand,q,a+h*i,std::placeholders::_1),0,2*M_PI,N);
	}
	
	return intErg*intErg;
}

//main
int main () {
	int N = 1000;
	std::string pfad = "/Users/johannesthewes/Git/CP/Blatt02/A2/";
	
	plot (std::bind(&fraunhofer,std::placeholders::_1,N),pfad,"Iq",51,0,5);	
}