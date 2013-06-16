#include <iostream>
#include <cmath>
#include <sstream>
#include <Eigen>
#include <fstream>

//Deklarieren der Lapack-Routinen
extern "C" void dsyev_(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info);

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

//Berechnet die Eigenwerte mittels der Lapack-Routinen und gibt diese auf dem Bildschirm aus
void lapack_dsyev(const int size, double *a, double *w, double *work, std::ofstream &datei) {
	int n = size;								//order of of the matrix
	int info;									//output variable
	int lda = n;								//the leading dimension of the array a
	int lwork = 4*size;							//the length of the array work
	char jobz[2] = {"N"};						//Compute only eigenvalues
	char uplo[2] = {"U"};						//Upper triangle of A is stored
	
	//Berechne Eigenwerte
	dsyev_(jobz, uplo, &n, a, &lda, w, work, &lwork, &info);
	
	//Ausgeben der Eigenwerte
	std::cout.precision(std::numeric_limits<double>::digits10);
	//std::cout << "Eigenwerte (mittels dsyev):" << std::endl;
	datei << "Eigenwerte (mittels dsyev):" << std::endl;
	for (int i = 0; i < size; i++) {
		if (i<10) {
			//std::cout << w[i] << std::endl;
			datei << w[i] << std::endl;
		}
	}	
}

//Hamiltonian für Ortsdarstellung (geschrieben in eine Eigen-Matrix)
Eigen::MatrixXd Hnm(double dXi, double lambdaS, int nmax) {
	Eigen::MatrixXd H(2*nmax+1,2*nmax+1);
	double dXi2 = dXi*dXi;
	for (int i = -nmax; i <= nmax; i++) {
		for (int j = -nmax; j <= nmax; j++) {
			int i2 = i*i;
			H(i+nmax,j+nmax) = -1/(dXi2)*(delta(i,j-1)+delta(i,j+1)-2*delta(i,j))+delta(i,j)*(dXi2*i2+lambdaS*dXi2*dXi2*i2*i2);
		}	
	}
	return H;
}

//Hamiltonian für Besetzungszahldarstellung
Eigen::MatrixXd Hnm(double lambdaS, int nmax) {
	Eigen::MatrixXd H(nmax+1,nmax+1);
	for (int i = 0; i <= nmax; i++) {
		for (int j = 0; j <= nmax; j++) {
			H(i,j) = (2*j+1)*delta(i,j)+lambdaS*1./4.*(sqrt(j*(j-1)*(j-2)*(j-3))*delta(i,j-4)+sqrt((j+1)*(j+2)*(j+3)*(j+4))*delta(i,j+4)+sqrt(j*(j-1))*(4*j-2)*delta(i,j-2)+sqrt((j+1)*(j+2))*(4*j+6)*delta(i,j+2)+(6*j*j+6*j+3)*delta(i,j));	
		}	
	}
	return H;	
}

//Eigenwerte für Ortseigenfunktionen berechnen und in Datei speichern
void OrtsEigen(double lambdaS, double dXi, std::ofstream &datei) {
	//Parameter für Ortshamilton
	double L = 10.;
	int nmax = floor(L/dXi+0.5);
	Eigen::MatrixXd H = Hnm(dXi, lambdaS, nmax);	//Hamiltonoperator
	
	int size = 2*nmax+1;							//Dimension der Matrix
	double *w = new double [size];					//the eigenvalues will be stored there
	double *work = new double [4*size];				//working directory
	
	//Matrix in 1D-Array speichern
	double *a = new double [size*size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			a[j*size+i] = H(i,j);
		}	
	}
	
	//Berechnung Eigenwerte Teil a.)
	lapack_dsyev(size, a, w, work, datei);
}

//Eigenwerte für Besetzungszahleigenfunktionen berechnen und in Datei speichern
void BesetzungszahlEigen(double lambdaS, int N, std::ofstream &datei2) {
	Eigen::MatrixXd Hn = Hnm(lambdaS, N);
	
	//Bestimmung der Eigenwerte mittels Lapack
	int size_b = N+1;								//Dimension der Matrix
	double *w_b = new double [size_b];				//the eigenvalues will be stored there
	double *work_b = new double [4*size_b];			//working directory
	
	//Matrix definieren
	double *a_b = new double [size_b*size_b];
	for (int i = 0; i < size_b; i++) {
		for (int j = 0; j < size_b; j++) {
			a_b[j*size_b+i] = Hn(i,j);
		}	
	}
	
	//Berechnung Eigenwerte Teil a.)
	lapack_dsyev(size_b, a_b, w_b, work_b, datei2);
}

int main () {
	//Globale Parameter
	const double lambdaS = 0.2;						//Lambda Schlange
	
	//---------------------------------------------------
	//						b.)
	//---------------------------------------------------
	std::vector<double> dXi = {0.1,0.05,0.025,0.0125};
	std::ofstream datei("Eigenwerte_Teila.txt");
	for (int i = 0; i < int(dXi.size()); i++) {
		datei << "DeltaXi: " << dXi[i] << std::endl;
		OrtsEigen(lambdaS, dXi[i], datei);
	}
	datei.close();
	
	//Mit Eigen kommen dieselben Werte raus...
	/*
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(H);
	std::cout << "The eigenvalues of A are:" << std::endl << es.eigenvalues() << std::endl;
	*/
	
	//---------------------------------------------------
	//						c.)
	//---------------------------------------------------
	std::vector<int> N = {50,70,90,110};
	std::ofstream datei2("Eigenwerte_Teilb.txt");
	for (int i = 0; i < int(N.size()); i++) {
		datei2 << "N: " << N[i] << std::endl;
		BesetzungszahlEigen(lambdaS, N[i], datei2);
	}
	datei2.close();
	
	return 0;	
}