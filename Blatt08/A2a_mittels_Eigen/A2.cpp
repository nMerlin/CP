#include <iostream>
#include <cmath>
#include <Eigen>
#include <fstream>

//Kronecker-Delta
double delta(int n, int m) {
	return ((n == m)?1.:0.);
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

int main() {
	//Parameter für Ortshamilton
	double lambdaS = 0.;
	double L = 10.;
	double dXi = 0.1;
	int nmax = floor(L/dXi+0.5);
	Eigen::MatrixXd H = Hnm(dXi, lambdaS, nmax);	//Hamiltonoperator
	
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(H);
	std::cout << "The eigenvalues of A are:" << std::endl << es.eigenvalues() << std::endl;

	return 0;
}