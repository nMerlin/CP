#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <cstring>
#include <fstream>
#include <omp.h>

//Plottet die vorgegebene Funktion (2D-Plot)
std::string plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop);

//Plottet die gegebene Datei in 2D
std::string plot(std::string pfad, std::string name);

//Plottet die gegebene Funktion (3D-Plot, symmetrisch in x und y)
std::string plot3d(std::function<double(double,double)> function, std::string pfad, std::string name, double schritte, double start, double stop);