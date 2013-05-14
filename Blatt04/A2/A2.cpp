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
	
	//Harmonischer Oszillator
	std::for_each(r_n.begin(), r_n.end(), [&Q, &A, &omega, &c, &v, &t_n](double &r_n){r_n = -v/Q*c - sin(r_n) + A*cos(omega*t_n);});
	
	//In der richtigen Reihenfolge wieder zusammensetzen
	ret.assign(v_n.begin(), v_n.end());
	ret.insert(ret.end(),r_n.begin(), r_n.end());
	
	return ret;
}

//Aufgabe 2 a.)
void A1a(std::vector<double> y, std::string name, double h) {
	int N = ceil(50/h);
	double Eges;

	std::ofstream a1_Eges(("./" + name + "_Eges.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(bind(&Pendel_b,std::placeholders::_1,std::placeholders::_2,1,0,0,0),y,h,h*i);
		Eges = (1-cos(y[0])) + y[1] * y[1] * 0.5; //Energieerhaltung bei harmonischem Oszillator
		
		a1_Eges << h*i << " " << Eges << std::endl;
	}
	
	a1_Eges.close();
	
	plot("./",name+"_Eges");
}

//Aufgabe 2 b.)
void A1b(std::vector<double> y, std::string name, double h) {
	int N = ceil(50./h);
	double theta_pi, theta_punkt;
	double A = 1.5, omega = 2./3., c=1.;
	double Q = 1.4;

	std::ofstream a2_praum(("./" + name + "_Phasenraum.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		if (y[0]>M_PI) {
			y[0] = y[0] - 2.*M_PI;
		} else if (y[0] < -M_PI) {
			y[0] = y[0] + 2.*M_PI;
		}
		
		y = Runge_Kutta(bind(&Pendel_b,std::placeholders::_1,std::placeholders::_2,Q,A,omega,c),y,h,h*i);
		theta_pi = y[0]/M_PI;
		theta_punkt = y[1];
		
		if (i%100 == 0) {
			a2_praum << theta_pi << " " << theta_punkt << std::endl;
		}
	}
	
	a2_praum.close();
	
	plot("./",name+"_Phasenraum");
}

int main() {
	//Variablen
	std::vector<double> y;
	double h = pow(10,-3);
	
	/*
	//Startparameter
	y.push_back(1);
	y.push_back(1);
	
	A1a(y, "A2a", h);
	*/
	
	y.push_back(0);
	y.push_back(0);
	
	A1b(y, "A2b", h);
	
	return 0;	
}