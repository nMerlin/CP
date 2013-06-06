#include "A1.hpp"

//Logistische Abbildung
double LogAbb(double r, double x_n) {
	return r*x_n*(1-x_n);	
}

//Kubische Abbildung
double KubAbb(double r, double x_n) {
	return (r * x_n - x_n * x_n * x_n);	
}

//Fortschrittsanzeige
void progress(double x) {
	std::cout << "\rProgress: " << x << "%";
	std::cout.flush(); //flush is required, otherwise, this would not work.
}

//Findet den Orbit der Funktion
std::vector<double> FindeOrbit(int init, int maxOrb, std::function<double(double,double)> Abb, double r, double x_start) {
	std::vector<double> orb;					//Speichert Orbit
	double x_n = x_start;
	
	//Initialisierung
	for (int i = 0; i < init; i++) {
		x_n = Abb(r, x_n);
	}
	
	//Orbit finden
	orb.push_back(x_n);
	for (int j = 0; j < maxOrb; j++) {
		x_n = Abb(r, x_n);
		if (float(x_n) == float(orb[0])) {
			return orb;	
		}
		orb.push_back(x_n);
	}
	
	
	return orb;	
}

int main() {
	//Parameter
	const int init = pow(10,5);					//initiale Zahl von Iterationen
	const int maxOrb = pow(10,3);				//Größter zulässiger Orbit
	const double logRmin = 3.4;					//Untere Grenze für den Parameter der logistischen Abb.
	const double logRmax = 4.;					//Oberer Grenzwert des Parameters der logistischen Abb. (in Vls.: 4.)
	const double kubRmin = 0.;
	const double kubRmax = 3.;					//Grenzwert des Parameters der kub. Abb.
	const double h = pow(10,-3);				//Diskretisierung von r
	
	//Variablen
	std::vector<double> orb;
	int nLog = (logRmax - logRmin)/h + 1;
	int nKub = (kubRmax - kubRmin)/h + 1;
	double r = 0;
	
	//Orbit der logistischen Abbildung
	std::ofstream dateiLog("LogAbb.dat");
	for (int iLog = 0; iLog < nLog; iLog++) {
		r = logRmin+iLog*h;
		orb = FindeOrbit(init, maxOrb, &LogAbb, r, 0.5);
		for (int i = 0; i < int(orb.size()); i++) {
			dateiLog << r << " " << orb[i] << std::endl;
		}
		progress(iLog/(nLog/100));
	}
	dateiLog.close();
	
	
	//Orbit der kubischen Abbildung
	std::ofstream dateiKub("KubAbb.dat");
	for (int iKub = 0; iKub < nKub; iKub++) {
		r = kubRmin+iKub*h;
		orb = FindeOrbit(init, maxOrb, &KubAbb, r, -0.5);
		for (int m = 0; m < int(orb.size()); m++) {
			dateiKub << r << " " << orb[m] << std::endl;
		}
		orb = FindeOrbit(init, maxOrb, &KubAbb, r, 0.5);
		for (int m = 0; m < int(orb.size()); m++) {
			dateiKub << r << " " << orb[m] << std::endl;
		}
		progress(iKub/(nKub/100));
	}
	dateiKub.close();
	
	return 0;	
}