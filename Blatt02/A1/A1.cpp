#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <limits>

//Funktion im Zähler der Hauptwertintegrale der Form (int_a^b dx (f(x))/(x-z))
double f(double x) {
	return exp(x);
}

//Integrand A1_a
double integrand(double x) {
	return f(x)/(x);
}

//I_Delta Integrand
double I_Delta(double x, double z, double Delta) {
	return (f(x*Delta+z)-f(z))/x;
}

//Teil b.)
double Gauss(double x) {
	return 2*exp(-x*x);
}

//Mittelpunktsregel
double mittelpunkt (/*Funktionspointer als Übergabeparameter double (*funcptr)(double)*/ std::function<double(double)> funcptr, double a, double b, double N) {
	
	//Variablen (intErg: Integrationsergebnis, h:Integrationsintervallbreite)
	double intErg = 0;
	double h = (b - a)/N;
	
	
	//Summe über alle mit 1 gewichteten Summanden
	for (int i = 0; i < N; i++) {
		intErg = intErg + funcptr(a+h*(2*i+1)/2);
	}
	
	//Rückgabe des Ergenbisses
	return h*intErg;
}

//Hauptwertintegral berechnen
double hauptwert(double (*funcptr)(double), std::function<double(double)> I_Delta_f, double a, double b, double z, double Delta, int N) {
	//***Variablen***
	double I_m = 0, I_p = 0, I_Delta;
	
	I_m = mittelpunkt(funcptr, a, z-Delta, N);
	I_p = mittelpunkt(funcptr, z+Delta, b, N);
	I_Delta = mittelpunkt(I_Delta_f, -1, 1, N);
	
	return I_m + I_p + I_Delta;
}

//Uneigentliches Integral aus Aufgabenteil A1 b.)
double integral_uneig(std::function<double(double)> integrand, double rel_fehler) {
	double f = 0., f_p = 1., b = 10., h = 1.;
	int i = 0, N = 0;
	
	while ((fabs(f-f_p)/f_p) > rel_fehler) {
		N = ceil(b/h);
		f = f_p;
		f_p = mittelpunkt(integrand, 0, b, N);
		
		if ((i % 2) == 0) {
			h = h/2;
		} else {
			b = 2*b;	
		}
		i++;
	}
	
	return f_p;
}

int main (int argc, char * const argv[]) {
	
	//***Variablendeklaration***
	
	//Integralgrenzen a,b und Singularität z, abgespaltene Umgebung Delta
	double a = -1;
	double b = 1; 
	double z = 0;
	double Delta = 0.0001;
	
	//Zahl der Stützstellen N (gerade!)
	int N = 10000;
	
	//bind durchlesen

	// binding functions:
	std::function<double (double)> I_Delta_f = std::bind(&I_Delta,std::placeholders::_1,z,Delta);
	
	std::cout
	<< std::setprecision(std::numeric_limits<double>::digits10)
    << hauptwert(&integrand, I_Delta_f, a, b, z, Delta, N) << std::endl
    << integral_uneig(&Gauss, pow(10,-5)) << std::endl;
    
    return 0;
}