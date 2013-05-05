//Einbinden der Standardbibliotheken
#include <iostream>
#include <cmath>
#include <vector>		// std::vector
#include <algorithm>	// std::for_each
#include <fstream>
#include <functional>

//Bindet die Funktionen zur Nutzung von Gnuplot ein
#include "./plot.hpp"
//Funktionen für das Runge-Kutta-Verfahren
#include "./Runge_Kutta.hpp"

//Rechte Seite des Differentialgleichungssystems
std::vector<double> F2 (double t_n, std::vector<double> y_n) {
	//Variablen
	const int y_l = y_n.size();
	double r = 0; 	//Betrag des Ortsvektors
	std::vector<double> ret, v_n, r_n;
	
	//Aufteilen in Ableitung von r und v
	v_n.assign(y_n.begin()+y_l/2,y_n.end());
	r_n.assign(y_n.begin(),y_n.end()-y_l/2);
	
	for (int i = 0; i < y_l/2; i++) {
		r += r_n[i]*r_n[i];
	}
	
	r = sqrt(r);
	
	std::for_each(r_n.begin(), r_n.end(), [&r](double &r_n){r_n = -r_n/(r*r*r);});
	
	ret.assign(v_n.begin(), v_n.end());
	ret.insert(ret.end(),r_n.begin(), r_n.end());
	
	return ret;
}

//Funktion nimmt gewünschte Anfangsbedingungen und erstellt passende Plots für Aufgabe 2
void A3a(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);
	//int size_y = y.size();
	std::ofstream a3_r(("./" + name + "_r.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F2,y,h,h*i);
		a3_r << y[0] << " " << y[1] << " " << y[2] << std::endl;
	}
	
	a3_r.close();
	
	plot3d("./",name+"_r");
}

int main() {
	//Variablen
	double h = pow(10,-4);
	std::vector<double> y;
	
	//Auswertung
	y.push_back(1);
	y.push_back(0);
	y.push_back(0);
	y.push_back(0);
	y.push_back(0.3);
	y.push_back(0);
	
	A3a(y, "03Ellipse", h);
	
	y[4] = 0.2;
	A3a(y, "02Ellipse", h);
	
	y[4] = 0.1;
	A3a(y, "01Ellipse", h);
	
	return 0;	
}