//Einbinden der Standardbibliotheken
#include <iostream>
#include <vector>		// std::vector
#include <algorithm>	// std::for_each
#include <fstream>
#include <functional>

//Bindet die Funktionen zur Nutzung von Gnuplot ein
#include "./plot.hpp"
//Funktionen für das Runge-Kutta-Verfahren
#include "./Runge_Kutta.hpp"

//Rechte Seite des Differentialgleichungssystems
std::vector<double> F1 (double t_n, std::vector<double> y_n) {
	//Variablen
	const int y_l = y_n.size();
	std::vector<double> ret, v_n, r_n;
	
	//Aufteilen in Ableitung von r und v
	v_n.assign(y_n.begin()+y_l/2,y_n.end());
	r_n.assign(y_n.begin(),y_n.end()-y_l/2);
	
	std::for_each(r_n.begin(), r_n.end(), [](double &r_n){r_n = -r_n;});
	
	ret.assign(v_n.begin(), v_n.end());
	ret.insert(ret.end(),r_n.begin(), r_n.end());
	
	return ret;
}

//Funktion nimmt gewünschte Anfangsbedingungen und erstellt passende Plots für Aufgabe 2
void A2(std::vector<double> y, std::string name, double h) {
	int N = ceil(100/h);
	int size_y = y.size();
	std::ofstream a2_r(("./" + name + "_r.dat").c_str());
	std::ofstream a2_v(("./" + name + "_v.dat").c_str());
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F1,y,h,h*i);
		if (size_y == 2) {
			a2_r << h*i << " " << y[0] << std::endl;
			a2_v << h*i << " " << y[1] << std::endl;
		} else if (size_y == 4) {
			a2_r << h*i << " " << y[0] << " " << y[1] << std::endl;
			a2_v << h*i << " " << y[2] << " " << y[3] << std::endl;
		}
	}
	
	a2_r.close();
	a2_v.close();
	
	if (size_y == 2) {
		plot("./",name+"_r");
		plot("./",name+"_v");
	} else if (size_y == 4) {
		plot3d("./",name+"_r");
		plot3d("./",name+"_v");
	}
}

int main() {
	
	// ***Variablen***
	//Schrittweite h für diskrete Zeitschritte
	double h;
	
	//Vektor y für Ort- und Geschwindigkeit zur Zeit t_n
	std::vector<double> y;
	
	// ***A2a***
	h = pow(10,-3);
	
	//Nachweis der harmonischen Schwingung (1D)
	y.push_back(1.);
	y.push_back(0);
	A2(y,"harm_oszi",h);
	
	//Zusätzlich Geschwindigkeitsbeitrag parallel zur Bewegungsrichtung (1D)
	y.clear();
	y.push_back(1.);
	y.push_back(1.);
	A2(y,"v_para",h);
	
	//Geschwindigkeit v nicht parallel zu r (2D)
	y.clear();
	y.push_back(1.);	//r_x
	y.push_back(0.);	//r_y
	y.push_back(0.);	//v_x
	y.push_back(1.);	//v_y
	A2(y,"v_antipara",h);
	
	return 0;
}