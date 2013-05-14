#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <cstring>
#include <fstream>
#include <omp.h>


class plotter
{
    private:
	    void init_();
	    void writeskript_();
	    void writedaten_();
	    
	    double xmin_, xmax_, ymin_, ymax_; //Plotbereich
	    double schrittweite_; //Schrittweite
	    int schritte_; //# Schritte
	    std::function<double(double)> function_;
	    
	    std::string pfad_; //Angabe wo die Datei gespeichert werden soll
	    std::string name_; //Name der Dateien
	    std::string title_; //Titel des Plots
	    std::string xlabel_;
	    std::string ylabel_;
	    
	    bool label_, yrange_;
	    
	    std::vector<std::string> daten_;
	    
      
    public:
	    plotter();
	    ~plotter();
	    
	  //setze Plotparameter
	  void setxrange(double xmin, double xmax);	  
	  void setyrange(double ymin, double ymax);	  
	  void setschritte(int schritte);
	  void setpfad(std::string pfad);	  
	  void setname(std::string name);	  
	  void settitle(std::string title);
	  void setxlabel(std::string xlabel);
	  void setylabel(std::string ylabel);
	  
	  void addDat(std::string pfad_name);
	  	  
	  //Plottet 2D Datensatz
	  int plotDat();
	  
	  //Plottet die vorgegebene Funktion (2D-Plot)
	  int plot(std::function<double(double)> function);
	  
	  int plot(std::function<double(double)> function, std::string pfad);
	  
	  int plot(std::function<double(double)> function, std::string pfad, std::string name);
	  
	  int plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte);
	  
	  int plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop);
	  
	  int plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop, std::string xlabel, std::string ylabel);
	  

	  //Plottet die gegebene Funktion (3D-Plot, symmetrisch in x und y)
	  std::string plot3d(std::function<double(double,double)> function, std::string pfad, std::string name, double schritte, double start, double stop);
  
};
#endif



