/*
Einfache Molekulardynamik Simulation in zwei Dimensionen mit paarweiser Lennard-Jones-Wechselwirkung
*/
#include "A1.hpp"

typedef struct {
	std::vector<double> g;
	int nhis;
	int ngr;
	double delg;
} Paarverteilung;

typedef struct {
	double v_sp;
	double T;
	double E_kin;
	double E_pot;
	double time;
} A1b;

typedef struct {
	double x;
	double y;
} geschwindigkeit;

typedef struct {
	double x;
	double y;
	double r2;
} ort;

typedef struct {
	//Ort
	double x;
	double y;
	//Geschwindigkeit
	double vx;
	double vy;
	//Äußere Kraft
	double fx;
	double fy;
} teilchen;

//Int-Konverter
std::string convertInt(int number)
{
   std::stringstream ss;		//create a stringstream
   ss << number;		//add number to the stream
   return ss.str();		//return a string with the contents of the stream
}

//Kleine Funktionen zum Debuggen
void debug_orte(std::vector<teilchen> p) {
	for (int i = 0; i < 16; i++) {
		std::cout << "x: " << p[i].x << "; y: " << p[i].y << std::endl;
	}
}

void debug_p(std::vector<teilchen> p) {
	for (int i = 0; i < 16; i++) {
		std::cout << "x: " << p[i].x << "; y: " << p[i].y << std::endl;
		std::cout << "vx: " << p[i].vx << "; vy: " << p[i].vy << std::endl;
		std::cout << "fx: " << p[i].fx << "; fy: " << p[i].fy << std::endl;
	}
}

void progress(double x) {
	std::cout << "\rProgress: " << x << "%";
	std::cout.flush(); //flush is required, otherwise, this would not work.
}

//Schreibe Ortsdaten in eine Datei (später in Animation überführt)
void gif_ort (std::vector<teilchen> p, int num) {
	std::ofstream datei(("Ort_"+convertInt(num)+".dat").c_str());
	
	for (int i = 0; i < 16; i++) {
		datei << p[i].x << " " << p[i].y << std::endl;	
	}
	
	datei.close();
}

//***Initialisierung der MD-Simulation***
std::vector<teilchen> init(double L, double T) {
	//Variablendeklaration
	geschwindigkeit v_sp = {0, 0};		//Schwerpunktsgeschwindigkeit
	double skal = 0;					//Skalierungsfaktor
	double E_kin = 0;					//Kinetische Energie
	const int N = 16;					//Anzahl der Teilchen
	
	std::vector<teilchen> p(N);			//Vektor mit den 16 betrachteten Teilchen
	
	//Definiere Startplätze und Zufallsgeschwindigkeiten der 16 Teilchen
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			p[4*i+j].x = 1./8.*(1.+2.*i)*L;							//x-Position in der Box
			p[4*i+j].y = 1./8.*(1.+2.*j)*L;							//y-Position in der Box
			p[4*i+j].vx = (float)rand()/(float)RAND_MAX - 0.5;		//Geschwindigkeit in x-Richtung zwischen -0.5 und 0.5
			p[4*i+j].vy = (float)rand()/(float)RAND_MAX - 0.5;		//Geschwindigkeit in y-Richtung zwischen -0.5 und 0.5
			v_sp.x += p[4*i+j].vx;												//Geschw aufaddieren
			v_sp.y += p[4*i+j].vy;												//Geschw aufaddieren
			E_kin += p[4*i+j].vx * p[4*i+j].vx + p[4*i+j].vy + p[4*i+j].vy;		//Gesamte Kinetische Energie berechnen
		}	
	}
	
	//Schwerpunktsgeschwindigkeit
	v_sp.x = v_sp.x/N;
	v_sp.y = v_sp.y/N;
	
	//Kinetische Energie
	E_kin = E_kin/N;
	T = T/N;
	
	//Skalierung der Geschwindigkeit um Schwerpunktsgeschwindigkeit zu eliminieren und Temperatur T anzuwenden
	skal = sqrt(2*T/E_kin);
	for (int i = 0; i < N; i++) {
		p[i].vx = (p[i].vx - v_sp.x)*skal;
		p[i].vy = (p[i].vy - v_sp.y)*skal;
	}
	
	return p;
}


//***Berechne Kräfte auf die Teilchen***
std::vector<teilchen> force(std::vector<teilchen> p, double r_c, double L, double &En) {
	//Variablendeklaration
	const int N = 16;
	const double rc2 = r_c * r_c;		//Quadrat des Cut-Off-Abstands
	const double E_cut = 0.;			//(Warum?)
	
	//Kräfte auf 0 setzen
	for (int i = 0; i < 16; i++) {
		p[i].fx = 0;
		p[i].fy = 0;	
	}
	
	//Energie Nullen
	En = 0;
	
	//Schleifen über alle Teilchenpaare (hier ist openmp wohl ziemlich hilfreich)
	//#pragma omp parallel for
	for (int i = 0; i < (N-1); i++) {
		for (int j = i+1; j < N; j++) {
			
			//Berechne Relativkoordinaten
			ort rel;
			rel.x = p[i].x - p[j].x;
			rel.y = p[i].y - p[j].y;
			
			//Wende periodische Randbedingungen an
			rel.x = rel.x - L * floor(rel.x/L+0.5);
			rel.y = rel.y - L * floor(rel.y/L+0.5);
			
			//Abstandsquadrat
			rel.r2 = rel.x * rel.x + rel.y * rel.y;
			
			//Cutoff testen und Kräfte berechnen
			if (rel.r2 < rc2) {
				double r2i = 1./rel.r2;
				double r6i = r2i * r2i * r2i;
				double ff = 48. * r2i * r6i * (r6i - 0.5);
				p[i].fx = p[i].fx + ff * rel.x;
				p[i].fy = p[i].fy + ff * rel.y;
				p[j].fx = p[j].fx - ff * rel.x;
				p[j].fy = p[j].fy - ff * rel.y;
				En = En + 4.*r6i*(r6i-1) - E_cut; //Segmentation fault
			}
		}	
	}
	return p;	
}

//***Integriere die Bewegungsgleichungen***
std::vector<teilchen> integrate (std::vector<teilchen> p, std::vector<teilchen> &pm, double h, double L, double &E_kin, geschwindigkeit &v_sp) {
	//Variablendeklaration
	std::vector<teilchen> xx = p;		//Teilchen-Vektor für Zwischenrechnung
	v_sp = {0,0};
	E_kin = 0;
		
	//#pragma omp parallel for
	for (int i = 0; i < 16; i++) {
		//Verlet-Algorithmus
		xx[i].x = 2 * p[i].x - pm[i].x + h * h * p[i].fx;
		xx[i].y = 2 * p[i].y - pm[i].y + h * h * p[i].fy;
		xx[i].vx = (xx[i].x - pm[i].x)/(2*h);
		xx[i].vy = (xx[i].y - pm[i].y)/(2*h);
		
		//Schwerpunktsgeschwindigkeit
		v_sp.x = v_sp.x + xx[i].vx;
		v_sp.y = v_sp.y + xx[i].vy;
		
		//Kinetische Energie
		E_kin = E_kin + xx[i].vx * xx[i].vx + xx[i].vy * xx[i].vy;
		
		//Periodische Randbedingungen
		while (xx[i].x >= L) {xx[i].x = xx[i].x - L; p[i].x = p[i].x - L;}
		while (xx[i].y >= L) {xx[i].y = xx[i].y - L; p[i].y = p[i].y - L;}
		while (xx[i].x < 0) {xx[i].x = xx[i].x + L; p[i].x = p[i].x + L;}
		while (xx[i].y < 0) {xx[i].y = xx[i].y + L; p[i].y = p[i].y + L;}
	}
	
	pm = p;
	E_kin = 0.5 * E_kin;

	return xx;
}

//***Parverteilung***
void gr(int sw, Paarverteilung &pv, std::vector<teilchen> p, double L) {
	const int N = 16;
	const double rho = N/(L*L);
	
	if (sw == 0) {								//***Initialisierung***
		pv.ngr = 0;								//Samplesize
		pv.delg = L/(2*pv.nhis);				//nhis muss vorher initialisiert worden sein
		for (int i = 0; i < pv.nhis; i++) {
			pv.g.push_back(0);
		}
	} else if (sw == 1) {						//***Stichprobe nehmen***
		pv.ngr = pv.ngr + 1;
		for (int i = 0; i < (N-1); i++) {
			for (int j = i+1; j < N; j++) {		//Schleife über alle Paare
				//Berechne Relativkoordinaten
				ort rel;
				rel.x = p[i].x - p[j].x;
				rel.y = p[i].y - p[j].y;
				
				//Wende periodische Randbedingungen an
				rel.x = rel.x - L * floor(rel.x/L+0.5);
				rel.y = rel.y - L * floor(rel.y/L+0.5);
				
				//Abstandsquadrat
				rel.r2 = rel.x * rel.x + rel.y * rel.y;
				double r = sqrt(rel.r2);
				
				//Wenn Abstand kleiner als halbe Box: Verteilung anpassen
				if (r < L/2) {
					int ig = floor((r+pv.delg/2)/pv.delg);
					pv.g[ig] = pv.g[ig] + 2;
				}
			}
		}
	} else if (sw == 2) {						//***g(r) bestimmen***
		for (int i = 0; i < pv.nhis; i++) {
			//double r = pv.delg*(i+0.5);						//Abstand r
			double vb = ((i+1)*(i+1)-i*i)*pv.delg*pv.delg;	//Volumen zwischen bin i+1 und i
			double nid = M_PI*vb*rho;						//Zahl der Teilchen im Volumen
			pv.g[i] = (pv.g[i]/(pv.ngr*N*nid));				//Normiere g(r)
		}
	}
}

//***Einfache MD-Simulation***
void md (double L, double h, double T, double r_c, double t_max) {
	//Variablendeklaration
	Paarverteilung pv;					//Paarverteilung
	pv.nhis = 1000;
	std::vector<teilchen> p;			//In diesem Vektor werden die Daten zu den "particles" gespeichert
	std::vector<teilchen> pm;			//Speichert vorherigen Zeitschritt für die Integration
	std::vector<A1b> a1b(1000);
	geschwindigkeit v_sp = {0, 0};		//Schwerpunktsgeschwindigkeit
	int j = 0, l=0;
	int sw = 0;
	double t = 0;						//Aktueller Zeitpunkt
	double En = 0;						//Potentielle Energie
	double E_kin = 0;					//Kinetische Energie
	double Temp = 0;					//Temperatur
	
	//Initialisierung
	p = init(L,T);						//Initialisierung der Simulation
	gr(sw, pv, p, L);					//Initialisierung der Paarverteilungsfkt
	sw = 1;
	
	//Setze Startwerte für den Verlet-Algorithmus
	p = force(p, r_c, L, En);
	pm = p;
	for (int i = 0; i < 16; i++) {
		pm[i].x = pm[i].x - h * pm[i].vx + 0.5 * h * h * pm[i].fx;
		pm[i].y = pm[i].y - h * pm[i].vy + 0.5 * h * h * pm[i].fy;	
	}
	
	//***Simulieren***
	while (t < t_max) {
		p = force(p, r_c, L, En);
		p = integrate(p, pm, h, L, E_kin, v_sp);
		
		//Messung Temperatur und Energie
		Temp = E_kin/(15);						//Temperatur
		//E_ges = (En + 0.5 * E_kin)/16;		//Gesamtenergie pro Teilchen
		
		//***Auswertungen***
		int i = floor(t/h+h/2.);				//Zählt die Iterationen
		//gif_ort(p, i);
		
		//Speichert die Werte für Aufgabenteil 1b
		if (i%2000 == 0) {
			progress(j/10);
			a1b[j].v_sp = sqrt(v_sp.x*v_sp.x+v_sp.y*v_sp.y);
			a1b[j].T = Temp;
			a1b[j].E_kin = 0.5 * E_kin/16.;
			a1b[j].E_pot = En/16.;
			a1b[j].time = t;
			j++;
		}
		
		//Animation über 100 Zeitschritte
		if ((i >= 1000000) && (i < 1000100)) {
			gif_ort(p, l);
			l++;
		}
		
		//Mittelung über 10**6 Zeitschritte (Äquilibriert ab ca. t = 10)
		if (i >= 1000000) {
			gr(sw, pv, p, L);	
		}
		
		t += h;
	}
	
	//Paarverteilung auswerten
	sw = 2;
	gr(sw, pv, p, L);
	std::ofstream pvdat("paarverteilung.dat");
	const int s_max = pv.g.size();
	for (int s = 0; s < s_max; s++) {
		double r = pv.delg*(s+0.5);
		pvdat << r << " " << pv.g[s] << std::endl;
	}
	pvdat.close();
	
	//Daten für A1b schreiben
	std::ofstream datei("a1b.dat");
	int k_max = a1b.size();
	for (int k = 0; k < k_max; k++) {
		datei
		<< a1b[k].time << " "
		<< a1b[k].v_sp << " "
		<< a1b[k].T << " "
		<< a1b[k].E_kin << " "
		<< a1b[k].E_pot << std::endl;
	}
	datei.close();
}

int main() {
	
	//Startparameter
	double h = 0.01;					//Vorgegebene Schrittweite
	double L = 8.;						//Kantenlänge der Box
	double T = 16.;						//Starttemperatur
	double r_c = 0.5 * L;				//Cutoff für die Kraftberechnung
	double t_max = 20000;				//Gesamtlaufzeit (Seltsame Abbrüche bei zu hoher Temperatur)
	
	md(L,h,T,r_c,t_max);
	
	return 0;	
}