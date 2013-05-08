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
	double alpha = 1;
	
	std::vector<double> ret, v_n, r_n;
	
	//Aufteilen in Ableitung von r und v
	v_n.assign(y_n.begin()+y_l/2,y_n.end());
	r_n.assign(y_n.begin(),y_n.end()-y_l/2);
	
	for (int i = 0; i < y_l/2; i++) {
		r += r_n[i]*r_n[i];
	}
	
	r = sqrt(r);
	
	//Gravitationspotential
	//std::for_each(r_n.begin(), r_n.end(), [&r](double &r_n){r_n = -r_n/(r*r*r);});
	
	//Alpha-Potential
	std::for_each(r_n.begin(), r_n.end(), [&r,&alpha](double &r_n){r_n = -r_n*alpha/(pow(r,alpha+2));});
	
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

//Berechnet den Betrag eines n-dimensionalen Vektors
double betrag (std::vector<double> v) {
	double summe = 0;
	int laenge = v.size();
	for (int i = 0; i < laenge; i++) {
		summe += v[i]*v[i];
	}
	return sqrt(summe);
}

//Berechnet den Drehimpuls aus dem Vektor (r,v) und gibt die Komponenten zurück
std::vector<double> L (std::vector<double> y) {
	std::vector<double> L;
	L.push_back(y[1]*y[5]-y[2]*y[4]);
	L.push_back(y[2]*y[3]-y[0]*y[5]);
	L.push_back(y[0]*y[4]-y[1]*y[3]);
	return L;	
}

//Berechnet den Lenz-Runge Vektor
std::vector<double> LenzRunge (std::vector<double> y) {
	std::vector<double> LR, _L;
	double r = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
	
	_L = L (y);
	LR.push_back(y[4]*_L[2]-y[5]*_L[1]-y[0]/r);
	LR.push_back(y[5]*_L[0]-y[3]*_L[2]-y[1]/r);
	LR.push_back(y[3]*_L[1]-y[4]*_L[0]-y[2]/r);
	
	return LR;	
}

//Erwartet mit y geeignete 3D-Anfangsbedingungen und erstellt Plots für Energie und Drehimpuls
void A3b(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);	//100 ist die Gesamtzeit
	double E_kin, E_pot, E_ges;
	double _L;
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
		_L = betrag(L(y));
		a3_r << y[0] << " " << y[1] << " " << y[2] << std::endl;
		a3_ekin << h*i << " " << E_kin << std::endl;
		a3_epot << h*i << " " << E_pot << std::endl;
		a3_eges << h*i << " " << E_ges << std::endl;
		a3_L << h*i << " " << _L << std::endl;
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

//Funktion nimmt gewünschte Anfangsbedingungen und erstellt passende Plots für Aufgabe 3c
void A3c(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);
	std::vector<double> LR;

	std::ofstream a3_r(("./" + name + "_r.dat").c_str());
	std::ofstream a3_LR_x(("./" + name + "_LR_x.dat").c_str());
	std::ofstream a3_LR_y(("./" + name + "_LR_y.dat").c_str());
	std::ofstream a3_LR_z(("./" + name + "_LR_z.dat").c_str());
	std::ofstream a3_LR(("./" + name + "_LR.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F2,y,h,h*i);
		
		LR = LenzRunge(y);
		
		a3_r << y[0] << " " << y[1] << " " << y[2] << std::endl;
		a3_LR_x << i*h << " " << LR[0] << std::endl;
		a3_LR_y << i*h << " " << LR[1] << std::endl;
		a3_LR_z << i*h << " " << LR[2] << std::endl;
		a3_LR << 0 << " " << 0 << " " << 0 << " " << LR[0] << " " << LR[1] << " " << LR[2] << std::endl;
	}
	
	a3_r.close();
	a3_LR_x.close();
	a3_LR_y.close();
	a3_LR_z.close();
	a3_LR.close();
	
	//plot3d("./",name+"_r");
	//plot("./",name+"_LR_x");
	//plot("./",name+"_LR_y");
	//plot("./",name+"_LR_z");
	plot3d("./",name+"_r");
}

//Funktion nimmt gewünschte Anfangsbedingungen und erstellt passende Plots für Aufgabe 3e
void A3e(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);
	int k = ceil(N/100);
	std::vector<double> LR;

	std::ofstream a3_r(("./" + name + "_r.dat").c_str());
	std::ofstream a3_LR_x(("./" + name + "_LR_x.dat").c_str());
	std::ofstream a3_LR_y(("./" + name + "_LR_y.dat").c_str());
	std::ofstream a3_LR_z(("./" + name + "_LR_z.dat").c_str());
	std::ofstream a3_LR(("./" + name + "_LR.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F2,y,h,h*i);
		
		LR = LenzRunge(y);
		
		a3_r << y[0] << " " << y[1] << " " << y[2] << std::endl;
		a3_LR_x << i*h << " " << LR[0] << std::endl;
		a3_LR_y << i*h << " " << LR[1] << std::endl;
		a3_LR_z << i*h << " " << LR[2] << std::endl;
		if ((i % k == 0)) {
			a3_LR << 0 << " " << 0 << " " << 0 << " " << LR[0] << " " << LR[1] << " " << LR[2] << std::endl;
		}
	}
	
	a3_r.close();
	a3_LR_x.close();
	a3_LR_y.close();
	a3_LR_z.close();
	a3_LR.close();
	
	//plot3d("./",name+"_r");
	plot("./",name+"_LR_x");
	plot("./",name+"_LR_y");
	plot("./",name+"_LR_z");
	//plot3d("./",name+"_r");
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
	y.push_back(0.6);
	y.push_back(0);
	
	//Die folgenden Funktionen funktionieren nur bedingt, je nach verlangtem Plot und alpha
	A3a(y, "Ellipse", h);
	//A3b(y, "Kepler", h);
	//A3c(y, "Lenz_Runge", h);
	//A3e(y, "Alpha_Potential", h);
	
	return 0;	
}