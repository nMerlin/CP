#ifndef TEILCHEN_H
#define TEILCHEN_H

#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <cstring>
#include <fstream>
#include <omp.h>


class Teilchen
{
    private:
	    void init_();
	    
	    std::vector<double> Ort_, Geschw_;
	    
      
    public:
	    Teilchen();
	    ~Teilchen();
	    
	  //setze Plotparameter
	  void setOrt(double x, double y);	  
	  void setOrt(std::vector<double> Ort);	  
	  void setGeschw(double xGeschw, double yGeschw);	  
	  void setGeschw(std::vector<double> Geschw);
	  
	  std::vector<double> Ort();
	  double xOrt();
	  double yOrt();
	  std::vector<double> Geschw();
	  double xGeschw();
	  double yGeschw();
	  
  
};
#endif

