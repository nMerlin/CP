#include <iostream>
#include <limits>
#include <cmath>
#include <fstream>
#include <Eigen> //Liegt in /opt/local/include/

//Deklarieren der Lapack-Routinen
extern "C" void dsyev_(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info);

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
	std::cout << "Eigenwerte (mittels dsyev):" << std::endl;
	datei << "Eigenwerte (mittels dsyev):" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << w[i] << std::endl;	
		datei << w[i] << std::endl;	
	}	
}

int main() {
	//Datei zur Aufnahme des Terminal-Outputs
	std::ofstream datei("Eigenwerte.txt");
	
	//Berechnung der Eigenwerte mittels LAPACK
	const int size = 4;							//Dimension der Matrix
	double w[size];								//the eigenvalues will be stored there
	double work[4*size];						//working directory
	
	//Matrix definieren
	double a[size*size] = {	1,2,2,4,
							2,5,2,1,
							2,2,6,3,
							4,1,3,4};
	Eigen::MatrixXd A(size,size);				//Matrix für Potenzmethode deklarieren
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
				A(i,j) = a[j*size+i];
			}	
	}
							
	//LAPACK:
	lapack_dsyev(size, a, w, work, datei);
	
	//Berechnung der Eigenwerte mittels der Potenzmethode:
	int n = 0;									//Misst Anzahl der Durchläufe
	double epsilon = pow(10,-14);				//Genauigkeitsziel
	double eigen0 = 0, eigen1 = 1;				//Speicher für Eigenwerte
	Eigen::VectorXd	e(size);					//Startvektor
	e(0)=1.;e(1)=2.;e(2)=3.;e(3)=4.;
	
	while ((fabs(eigen0-eigen1) > epsilon)) {
		eigen0 = eigen1;
		eigen1 = e(0);
		e = A*e;
		eigen1 = e(0)/eigen1;
		e = e/(e.dot(e));
		n++;
	}
	
	std::cout << "Der größte Eigenwert nach der Potenzmethode lautet: " << eigen1 << std::endl;
	datei << "Der größte Eigenwert nach der Potenzmethode lautet: " << eigen1 << std::endl;
	std::cout << "Die Genauigkeit von 10^-14 wurde nach " << n << " Durchläufen erreicht." << std::endl;
	datei << "Die Genauigkeit von 10^-14 wurde nach " << n << " Durchläufen erreicht." << std::endl;
	
	return 0;	
}