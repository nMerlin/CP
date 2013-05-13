#include "Teilchen.h"

Teilchen::Teilchen()
{
  init_();
}
Teilchen::~Teilchen(){}

void Teilchen::init_()
{
    
}

void Teilchen::setOrt(double x, double y)
{
  Ort_[0] = x;
  Ort_[1] = y;
}	  
void Teilchen::setOrt(std::vector<double> Ort)
{
  Ort_ = Ort;
}	  
void Teilchen::setGeschw(double xGeschw, double yGeschw)
{
  Geschw_[0] = xGeschw;
  Geschw_[1] = yGeschw;
}	  
void Teilchen::setGeschw(std::vector<double> Geschw)
{
  Geschw_ = Geschw;
}

std::vector<double> Teilchen::Ort()
{
  return Ort_;
}
double Teilchen::xOrt()
{
  return Ort_[0];
}
double Teilchen::yOrt()
{
  return Ort_[1];  
}
std::vector<double> Teilchen::Geschw()
{
  return Geschw_;
}
double Teilchen::xGeschw()
{
  return Geschw_[0];
}
double Teilchen::yGeschw()
{  
  return Geschw_[1];
}
