#include "A2.hpp"

typedef struct {
	double min;
	double max;
} schranke;

//Logistische Abbildung
double LogAbb(double r, double x_n) {
	return r*x_n*(1-x_n);	
}

//Gn aus der Aufgabenstellung
double Gn(int n, double r) {
	double xn=0.5;								//Startwert der Iteration
	int nmax = pow(2,n);
	for (int i = 0; i < nmax; i++) {			//Bildet 2^n Iterationen der logistischen Abbildung
		xn = LogAbb(r, xn);
	}
	return 0.5 - xn;
}

//Regula Falsi
schranke Intervallhalbierung(schranke s, double epsilon, int n) {
	//Variablen
	double z;
	
	//Iteration
	while ((s.max-s.min)>epsilon) {
		z = (s.min+s.max)/2;
		if ((Gn(n,z)*Gn(n,s.min)) < 0) {
			s.max = z;
		} else {
			s.min = z;	
		}
	}
	return s;
}

//Daten in eine Datei schreiben
void SchreibeDatenFkt(std::function<double(double)> f, double min, double max, double h, std::string name) {
	int nmax = ceil((max-min)/h);
	double x = min;
	std::ofstream datei(name+".dat");
	for (int i = 0; i < nmax; i++) {
		datei << x << " " << f(x) << std::endl;
		x += h;
	}
	datei.close();
}

int main() {
	//Parameter
	const double r_inf = 3.57;				//Obergrenze für r
	const double h_a = pow(10,-4);			//Schrittweite im Aufgabenteil a.)
	const double epsilon = pow(10,-13);		//Genauigkeitsziel bei der Nullstellensuche
	
	//Variablen
	std::vector<schranke> schranken;
	std::vector<double> R;					//Speichert Nullstellen von g3
	double delta;							//Feigenbaum-Konstante
	schranke s;
	
	//Aufgabenteil a.)
	SchreibeDatenFkt(std::bind(&Gn,0,std::placeholders::_1),0,r_inf,h_a,"A2a_n0");
	SchreibeDatenFkt(std::bind(&Gn,1,std::placeholders::_1),0,r_inf,h_a,"A2a_n1");
	SchreibeDatenFkt(std::bind(&Gn,2,std::placeholders::_1),0,r_inf,h_a,"A2a_n2");
	SchreibeDatenFkt(std::bind(&Gn,3,std::placeholders::_1),0,r_inf,h_a,"A2a_n3");
	
	//Abgelesene Schranken für die Nullstellen
	s.min = 1.95; s.max = 2.1; schranken.push_back(s);
	s.min = 3.1; s.max = 3.3; schranken.push_back(s);
	s.min = 3.45; s.max = 3.55; schranken.push_back(s);
	s.min = 3.55; s.max = 3.7; schranken.push_back(s);
	
	//Nullstellen bestimmen und ausgeben
	std::ofstream datei("Nullstellen.dat");
	datei << "#n R1 R2 R3" << std::endl;
	datei.precision(std::numeric_limits<double>::digits10);
	for (int n = 1; n < 4; n++) {
		datei << n << " ";
		for (int i = 0; i < (n+1); i++)	{
			double nst = Intervallhalbierung(schranken[i], epsilon, n).min;
			datei << nst << " ";
			if (n==3) {
				R.push_back(nst);	
			}
		}
		datei << std::endl;
	}
	
	//Schätzung für die Feigenbaum-Konstante
	delta = (R[2]-R[1])/(R[3]-R[2]);
	std::cout << "delta: " << delta << std::endl;
	datei << "Schätzung Feigenbaum-Konstante: " << delta << std::endl;
	
	return 0;	
}