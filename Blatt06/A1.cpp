#include "A1.hpp"

//Int-Konverter
std::string convertInt(int number)
{
   std::stringstream ss;		//create a stringstream
   ss << number;		//add number to the stream
   return ss.str();		//return a string with the contents of the stream
}

//Kronecker-Delta
double delta(int n, int m) {
	return ((n == m)?1.:0.);
}

//Diskretisierter Hamiltonoperator nach Crank-Nicholson-Algrithmus
double H(int n, int m, double dXi) {
	double dXi2 = dXi*dXi;
	double dnm = delta(n,m);
	return -1/(dXi2)*(delta(n,m-1) + delta(n,m+1) - 2*dnm) + dXi2*n*n*dnm;
}

//Diskretisierter Zeitentwicklungsoperator
Eigen::MatrixXcd S_H(double deltaT, std::function<double(int,int,double)> H, double dXi, double xiMax, double xiMin) {
	int n_max = floor((xiMax - xiMin + 3/2*dXi)/dXi);
	Eigen::MatrixXd mH(n_max,n_max);
	Eigen::MatrixXcd mS_H(n_max,n_max), mTemp(n_max,n_max), mTempi(n_max,n_max);
	
	for (int i = 0; i < n_max; i++) {
		for (int j = 0; j < n_max; j++) {
			mH(i,j) = H((i-floor(n_max/2)),(j-floor(n_max/2)),dXi);
		}	
	}
	
	mTemp.real() = Eigen::MatrixXd::Identity(n_max,n_max);
	mTemp.imag() = -0.5*mH*deltaT;
	
	mTempi.real() = Eigen::MatrixXd::Identity(n_max,n_max);
	mTempi.imag() = 0.5*mH*deltaT;
	
	mS_H = mTempi.inverse() * mTemp;
	
	return mS_H;
}

//Anfangszustand
double Psi(double xi, double xiNull, double sigma) {
	double c = (2*M_PI*sigma);
	double c4 = c*c*c*c;
	double c4i = 1/c4;
	
	return c4i * exp(-(xi-xiNull)*(xi-xiNull)/(4*sigma));
}

//Zustandsvektor in Datei schreiben
void SchreibeZustand(Eigen::VectorXcd xi, std::string name) {
	int n_max = xi.size();
	std::ofstream datei(name + ".dat");
	for (int i = 0; i < n_max; i++) {
		datei << (i - floor(n_max/2)) << " " << abs(xi(i))*abs(xi(i)) << std::endl;	
	}
	datei.close();
}

//Norm eines Zustands
double Norm(Eigen::VectorXcd xi) {
	double summe = 0;
	int n_max = xi.size();
	for (int i = 0; i < n_max; i++) {
		summe += abs(xi(i))*abs(xi(i));	
	}
	return summe;	
}

//Initialisierung
Eigen::VectorXcd init(double xiMax, double xiMin, double dXi) {
	int n_max = floor((xiMax - xiMin + 3/2*dXi)/dXi);
	double summe = 0;
	double skal = 0;
	Eigen::VectorXcd xi(n_max);
	std::complex<double> c;
	
	for (int i = 0; i < n_max; i++) {
		c = Psi((i-floor(n_max/2))*dXi,1,1);
		xi(i) = c;
	}
	
	//Numerische Normierung des Anfangszustandes auf 1
	summe = Norm(xi);
	skal = sqrt(1./summe);
	xi = skal * xi;
	
	//Anfangszustand in Datei schreiben
	SchreibeZustand(xi, "A1c");
	
	return xi;
}

//Ableitung der Wellenfunktion an der Stelle n
std::complex<double> Ableitung(Eigen::VectorXcd xi, int n, double deltaXi) {
	int laenge = xi.size();
	
	if (n == 0) {
		return (xi(1)-xi(0))/(deltaXi);
	} else if (n == (laenge-1)) {
		return (xi(laenge-1) - xi(laenge-2))/(deltaXi);
	} else {
		return (xi(n+1)-xi(n-1))/(2.*deltaXi);
	}	
}

//Zweite Ableitung der Wellenfunktion an der Stelle n
std::complex<double> Ableitung2(Eigen::VectorXcd xi, int n, double deltaXi) {
	int laenge = xi.size();
	
	if (n == 0) {
		return 0;
	} else if (n == (laenge-1)) {
		return 0;
	} else {
		return (xi(n+1)-2.*xi(n)+xi(n-1))/(deltaXi*deltaXi);
	}	
}

//Main-Function
int main () {
	double xiMax = 10;
	double xiMin = -10;
	double deltaXi = 0.1;
	double deltaT = 0.05;
	double mittelWert = 0, mittelWert2 = 0;
	double impuls = 0, impuls2 = 0;
	int cn = floor(10./deltaT+0.5);
	int iN = 0;
	std::complex<double> c_i = (0,1);
	
	Eigen::MatrixXcd Sh = S_H(deltaT, &H, deltaXi, xiMax, xiMin);
	Eigen::VectorXcd xi = init(xiMax, xiMin, deltaXi);
	Eigen::VectorXcd xi_n = xi;
	Eigen::VectorXcd xi_temp;
	
	//Dateistreams für geforderte Ausgabe-Plots
	std::ofstream datei("A1d_Normierung.dat");
	std::ofstream datei2("A1f.dat");
	
	//Anwenden des Zeitentwicklungsoperators bis t=10
	for (int i = 0; i < cn; i++) {
		xi_n = Sh*xi_n;
		datei << i*deltaT << " " << Norm(xi_n) << std::endl;
		
		//Animationssequenz
		//if (i%2 == 0) {
		//	SchreibeZustand(xi_n, "A1d_Animation_"+convertInt(i/2));
		//}
		
		//Mittelwert des Ortes und des Impulses
		for (int j = 0; j < xi.size(); j++) {
			iN = j - floor(xi.size()/2);
			mittelWert += iN*deltaXi*abs(xi_n(j))*abs(xi_n(j));
			mittelWert2 += (iN*deltaXi)*(iN*deltaXi)*abs(xi_n(j))*abs(xi_n(j));
			impuls += abs(xi_n(j)*(-c_i)*Ableitung(xi_n, j, deltaXi));
			impuls2 += abs(xi_n(j)*(-1.)*Ableitung2(xi_n, j, deltaXi));
		}
		datei2 << i*deltaT << " " << mittelWert << " " << (mittelWert2 - mittelWert*mittelWert) << " " << impuls << " " << impuls2 << std::endl;
		mittelWert = 0;
		mittelWert2 = 0;
		impuls = 0;
		impuls2 = 0;
	}
	
	datei.close();
	datei2.close();
	
	SchreibeZustand(xi_n, "A1d_Zustand");
	
	return 0;	
}