//Einbinden der Standardbibliotheken
#include <iostream>
#include <vector>		// std::vector
#include <algorithm>	// std::for_each
#include <fstream>

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


int main() {
	
	//***Variablen***
	//Schrittweite h für diskrete Zeitschritte
	double h;
	int N;
	//Vektor y für Ort- und Geschwindigkeit zur Zeit t_n
	std::vector<double> y;
	
	//***A2a***
	//Nachweis der harmonischen Schwingung (1D-Anfangsbedingungen)
	h = pow(10,-3);
	N = ceil(100/h);
	y.push_back(1.);
	y.push_back(0);
	
	std::ofstream harm_oszi("./harm_oszi.dat");
	
	for (int i = 0; i < N; i++) {
		y = Runge_Kutta(&F1,y,h,h*i);
		harm_oszi << h*i << " " << y[0] << std::endl;
	}
	harm_oszi.close();
	plot("./","harm_oszi");
	
	return 0;
}