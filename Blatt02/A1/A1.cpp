#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>

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

int main (int argc, char * const argv[]) {
	
	//***Variablendeklaration***
	
	//Integralgrenzen a,b und Singularität z, abgespaltene Umgebung Delta
	double a = -1;
	double b = 1; 
	double z = 0;
	double Delta = 0.0001;
	
	//Zahl der Stützstellen N (gerade!)
	int N = 1000;
	
	//bind durchlesen

	// binding functions:
	std::function<double (double)> I_Delta_f = std::bind(&I_Delta,std::placeholders::_1,z,Delta);
	
    std::cout << hauptwert(&integrand, I_Delta_f, a, b, z, Delta, N) << std::endl;
    
    return 0;
}