//Alles wird nur einmal eingebunden
#pragma once

//Includes
#include <vector>
#include <functional>
#include <iostream>

//Hilfsfunktion um Zwischenwerte des Runge-Kutta-Verfahrens zu berechnen
std::vector<double> addiere1halb (std::vector<double> y_n, std::vector<double> k, double h);

//Führt einen Iterationsschritt des Runge-Kutta-Verfahrens durch
std::vector<double> Runge_Kutta (std::function<std::vector<double>(double,std::vector<double>)> F, std::vector<double> y_n, double h, double t_n);