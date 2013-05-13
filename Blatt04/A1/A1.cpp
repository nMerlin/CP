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
std::vector<double> Pendel_a (double t_n, std::vector<double> y_n, double Q, double A, double omega, double c) {
	//Variablen
	const int y_l = y_n.size();
	double v;
	
	std::vector<double> ret, v_n, r_n;
	
	//Aufteilen in Ableitung von r und v
	v_n.assign(y_n.begin()+y_l/2,y_n.end());
	r_n.assign(y_n.begin(),y_n.end()-y_l/2);
	
	v = v_n[0];
	
	//Harmonischer Oszillator
	std::for_each(r_n.begin(), r_n.end(), [&Q, &A, &omega, &c, &v, &t_n](double &r_n){r_n = -v/Q*c - r_n + A*cos(omega*t_n);});
	
	//In der richtigen Reihenfolge wieder zusammensetzen
	ret.assign(v_n.begin(), v_n.end());
	ret.insert(ret.end(),r_n.begin(), r_n.end());
	
	return ret;
}

//Aufgabe 1 a
void A1a(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);
	double Eges;

	std::ofstream a1_Eges(("./" + name + "_Eges.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(bind(&Pendel_a,std::placeholders::_1,std::placeholders::_2,1,0,0,0),y,h,h*i);
		Eges = 0.5 * y[1] * y[1] + 0.5 * y[0] * y[0];
		
		a1_Eges << h*i << " " << Eges << std::endl;
	}
	
	a1_Eges.close();
	
	plot("./",name+"_Eges");
}

int main() {
	//Variablen
	std::vector<double> y;
	double h = pow(10,-1);
	
	//Startparameter
	y.push_back(1);
	y.push_back(1);
	
	A1a(y, "A1_Energieerhalt", h);
	
	return 0;	
}