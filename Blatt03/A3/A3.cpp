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

//Funktion nimmt gewünschte Anfangsbedingungen und erstellt passende Plots für Aufgabe 2a
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

//Erwartet mit y geeignete 3D-Anfangsbedingungen und erstellt Plots für Energie und Drehimpuls
void A3b(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);	//100 ist die Gesamtzeit
	double E_kin, E_pot, E_ges;
	double L_x, L_y, L_z, L;
	//int size_y = y.size();
	std::ofstream a3_r(("./" + name + "_r.dat").c_str());
	std::ofstream a3_ekin(("./" + name + "_ekin.dat").c_str());
	std::ofstream a3_epot(("./" + name + "_epot.dat").c_str());
	std::ofstream a3_eges(("./" + name + "_eges.dat").c_str());
	std::ofstream a3_L(("./" + name + "_L.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F2,y,h,h*i);
		E_kin = 0.5*(y[3]*y[3]+y[4]*y[4]+y[5]*y[5]);
		E_pot = -1/sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
		E_ges = E_kin + E_pot;
		L_x = y[1]*y[5]-y[2]*y[4];
		L_y = y[2]*y[3]-y[0]*y[5];
		L_z = y[0]*y[4]-y[1]*y[3];
		L = sqrt(L_x*L_x+L_y*L_y+L_z*L_z);
		a3_r << y[0] << " " << y[1] << " " << y[2] << std::endl;
		a3_ekin << h*i << " " << E_kin << std::endl;
		a3_epot << h*i << " " << E_pot << std::endl;
		a3_eges << h*i << " " << E_ges << std::endl;
		a3_L << h*i << " " << L << std::endl;
	}
	
	a3_r.close();
	a3_ekin.close();
	a3_epot.close();
	a3_eges.close();
	a3_L.close();
	
	plot3d("./",name+"_r");
	plot("./",name+"_ekin");
	plot("./",name+"_epot");
	plot("./",name+"_eges");
	plot("./",name+"_L");
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
	y.push_back(0.7);
	y.push_back(0);
	
	A3b(y, "Kepler", h);
	
	return 0;	
}