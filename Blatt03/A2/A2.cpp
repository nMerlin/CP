//Einbinden der Standardbibliotheken
#include <iostream>
#include <vector>		// std::vector
#include <algorithm>	// std::for_each

//Bindet die Funktionen zur Nutzung von Gnuplot ein
#include "./plot.hpp"

//Kraftfeld F(r) für das die Newtonschen Bewegungsgleichungen gelöst werden
std::vector<double> F1 (std::vector<double> r) {
	std::for_each(r.begin(), r.end(), [](double r_i) -> double {return -r_i;});
	return r;
}

int main() {
	
	//***Variablen***
	
	//Schrittweite h für diskrete Zeitschritte
	const double h = pow(10,-3);
	
	//Anfangsbedingung für den Ortsvektor
	std::vector<double> r;
	r.assign (3,1.);
	
	std::cout << h << std::endl;
	
	return 0;
}