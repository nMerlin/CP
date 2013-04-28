#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <cstring>
#include <fstream>

//Plottet die vorgegebene Funktion
std::string plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop);