//Einbinden der Standardbibliotheken
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>		// std::vector
#include <algorithm>	// std::for_each
#include <fstream>
#include <functional>
#include <cstring>

//Einbinden der eigenen Hilfsbibliotheken
#include "Runge_Kutta.hpp"
#include "plot.hpp"

//Rechte Seite des Differentialgleichungssystems
std::vector<double> Pendel_b (double t_n, std::vector<double> y_n, double Q, double A, double omega, double c) {
	//Variablen
	const int y_l = y_n.size();
	double v;
	
	std::vector<double> ret, v_n, r_n;
	
	//Aufteilen in Ableitung von r und v
	v_n.assign(y_n.begin()+y_l/2,y_n.end());
	r_n.assign(y_n.begin(),y_n.end()-y_l/2);
	
	v = v_n[0];
	
	//Harmonischer Oszillator 3a
	//std::for_each(r_n.begin(), r_n.end(), [&Q, &A, &omega, &c, &v, &t_n](double &r_n){r_n = -v/Q*c - r_n + A*cos(omega*t_n);});
	
	//Nichtlineares Pendel 3b
	std::for_each(r_n.begin(), r_n.end(), [&Q, &A, &omega, &c, &v, &t_n](double &r_n){r_n = -v/Q*c - sin(r_n) + A*cos(omega*t_n);});
	
	//In der richtigen Reihenfolge wieder zusammensetzen
	ret.assign(v_n.begin(), v_n.end());
	ret.insert(ret.end(),r_n.begin(), r_n.end());
	
	return ret;
}

//Aufgabe 3 a.)
void A3a(std::vector<double> y, std::string name, double h) {
	int N = ceil(1000/h);
	int n = 0;

	std::ofstream a3_Schnitt(("./" + name + "_PoincareSchnitt.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(bind(&Pendel_b,std::placeholders::_1,std::placeholders::_2,1,0,0,0),y,h,h*i);
		
		if (ceil(h*i/(2.*M_PI)) != n) {
			a3_Schnitt << y[0]/M_PI << " " << y[1] << std::endl;
			n += 1;
		}
		
		/*
		if (i%100 == 0) {
			a3_Schnitt << y[0] << " " << y[1] << std::endl;	
		}*/
	}
	
	a3_Schnitt.close();
	
	plot("./",name+"_PoincareSchnitt");
}

//Aufgabe 3 b.)
double A3b(std::vector<double> y, double h, double Q) {
	int N = ceil(50./h);
	int n = 0;
	double A = 1.5, omega = 2./3., c=1.;
	double ret = 0;

	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(bind(&Pendel_b,std::placeholders::_1,std::placeholders::_2,Q,A,omega,c),y,h,h*i);
		if (ceil(h*i/(3.*M_PI)) != n) {
			ret = y[1];
			n += 1;
		}
	}
	
	return ret;
}

int main() {
	//Variablen
	std::vector<double> y;
	double h = pow(10,-3);
	double Q = 0.5;
	int N3b = 100, N3b2 = 100;
	double h3b = (1.3-0.5)/N3b;
	double h3b2 = 0.1/N3b2;
	std::string name = "A3b";
	
	std::ofstream a3_schnitt(("./" + name + "_PoincareSchnitt.dat").c_str());
	
	/*
	//Startparameter
	y.push_back(1);
	y.push_back(1);
	
	A3a(y, "A3a", h);
	*/
	
	y.push_back(0);
	y.push_back(-3);
	
	for (int i=0; i < (N3b+N3b2); i++) {
		a3_schnitt << Q << " " << A3b(y, h, Q) << std::endl;
		if (i<N3b) {
			Q += h3b;
		} else {
			Q += h3b2;
		}
	}
	
	a3_schnitt.close();
	plot("./",name+"_PoincareSchnitt");
	
	return 0;	
}