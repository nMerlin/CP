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
		return cos(q*r*cos(phi))*r;
}

//Integrand für Viertelkreis
double integrand_viertel_real (double q_x, double q_y, double r, double phi) {
		double q = sqrt(q_x*q_x + q_y*q_y);
		double theta = atan(q_y/q_x);
		
		return cos(q*r*cos(phi-theta))*r;
}

double integrand_viertel_imag (double q_x, double q_y, double r, double phi) {
		double q = sqrt(q_x*q_x + q_y*q_y);
		double theta = atan(q_y/q_x);
		
		return -sin(q*r*cos(phi-theta))*r;
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

//Berechnet Beugungsintegral für 1/4-Kreisstück
double fraunhofer_viertel (double q_x, double q_y, int N) {
	//***Variablen***
	double a = 1;
	double intErg_imag = 0;
	double intErg_real = 0;
	double h = a/N;
	
	//***Integration***
	//Randwerte Realteil
	intErg_real = intErg_real + h/3 * (simpson(std::bind(&integrand_viertel_real,q_x,q_y,a,std::placeholders::_1),0,M_PI/2,N) + simpson(std::bind(&integrand_viertel_real,q_x,q_y,2*a,std::placeholders::_1),0,M_PI/2,N));
	
	//Randwerte Imaginärteil
	intErg_imag = intErg_imag + h/3 * (simpson(std::bind(&integrand_viertel_imag,q_x,q_y,a,std::placeholders::_1),0,M_PI/2,N) + simpson(std::bind(&integrand_viertel_real,q_x,q_y,2*a,std::placeholders::_1),0,M_PI/2,N));
	
	//Für mehrdimensionale Integration wird zweimal die simpson-Regel angewendet
	//Summe über die mit 4/3 und 2/3 gewichteten Summanden
	for (int i = 1; i < N; i++) {
		intErg_real += h * 2/3 * ((i % 2) + 1) * simpson(std::bind(&integrand_viertel_real,q_x,q_y,a+h*i,std::placeholders::_1),0,M_PI/2,N);
		intErg_imag += h * 2/3 * ((i % 2) + 1) * simpson(std::bind(&integrand_viertel_imag,q_x,q_y,a+h*i,std::placeholders::_1),0,M_PI/2,N);
	}
	
	return intErg_real*intErg_real + intErg_imag*intErg_imag;
}

//main
int main () {
	int N = 100;
	std::string pfad = "/Users/johannesthewes/Git/CP/Blatt02/A2/";
	
	plot (std::bind(&fraunhofer,std::placeholders::_1,N),pfad,"Iq",51,0,5);
	plot3d (std::bind(&fraunhofer_viertel,std::placeholders::_1,std::placeholders::_2,N),pfad,"Iqxqy",61,-6,6);
}