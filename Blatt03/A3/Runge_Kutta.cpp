#include "./Runge_Kutta.hpp"

//Hilfsfunktion um Zwischenwerte des Runge-Kutta-Verfahrens zu berechnen
std::vector<double> addiere1yn (std::vector<double> y_n, std::vector<double> k, double h, double faktor) {
	std::vector<double> ret;
	const int v_l = y_n.size();
	for (int i = 0; i < v_l; i++) {
		ret.push_back(y_n[i] + faktor * h * k[i]);
	}
	return ret;
}

//Führt einen Iterationsschritt des Runge-Kutta-Verfahrens durch
std::vector<double> Runge_Kutta (std::function<std::vector<double>(double,std::vector<double>)> F, std::vector<double> y_n, double h, double t_n) {
	//y_np1 ist das Ergebnis des nächsten Schrittes, k1, ..., k4 enthalten die Zwischenschritte
	std::vector<double> y_np1, k1, k2, k3, k4;
	const int v_l = y_n.size();
		
	//Zwischenschritte, h wird erst komponentenweise später multipliziert
	k1 = F(t_n, y_n);
	k2 = F(t_n + h/2, addiere1yn(y_n, k1, h, 0.5));
	k3 = F(t_n + h/2, addiere1yn(y_n, k2, h, 0.5));
	k4 = F(t_n + h, addiere1yn(y_n, k3, h, 1));
	
	//y_n+1 wird ausgerechnet
	for (int i = 0; i < v_l; i++) {
		y_np1.push_back(y_n[i]+h/6*(k1[i]+2*k2[i]+2*k3[i]+k4[i]));	
	}
	
	return 	y_np1;
}