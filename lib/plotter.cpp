#include "plotter.hpp"

plotter::plotter()
{
  init_();
}
plotter::~plotter(){}

void plotter::init_()
{
    xmin_ = 0.;
    xmax_ = 1.;
    schritte_ = 100;
    schrittweite_ = (xmax_ - xmin_)/schritte_;
    pfad_ = "./";
    name_ = "name";
    title_ = "title";
    xlabel_ = "x";
    ylabel_ = "y";
    label_ = false;
    yrange_ = false;
}

void plotter::setxrange(double xmin, double xmax)
{
    xmin_ = xmin;
    xmax_ = xmax;
    schrittweite_ = (xmax_ - xmin_)/schritte_;
}

void plotter::setyrange(double ymin, double ymax)
{
    ymin_ = ymin;
    ymax_ = ymax;
    yrange_ = true;
}

void plotter::setschritte(int schritte)
{
    schritte_ = schritte;
    schrittweite_ = (xmax_ - xmin_)/schritte_;
}

void plotter::setpfad(std::string pfad)
{
    pfad_ = pfad;
}

void plotter::setname(std::string name)
{
    name_ = name;
}

void plotter::settitle(std::string title)
{
    title_ = title;
}

void plotter::setxlabel(std::string xlabel)
{
    xlabel_ = xlabel;
    label_ = true;
}

void plotter::setylabel(std::string ylabel)
{
    ylabel_ = ylabel;
    label_ = true;
}

void plotter::writeskript_()
{
    std::ofstream skript((pfad_ + "skript_" + name_ + ".plt").c_str());
    
    skript << "set grid" << std::endl;
    skript << "set terminal pdfcairo" << std::endl;
    skript << "set terminal pdfcairo enhanced" << std::endl;
    skript << "set output '" + pfad_ + name_ + ".pdf'" << std::endl;
    skript << "set xrange [" << xmin_ << ":" << xmax_ << "]" << std::endl;
    if(yrange_)
    {
	skript << "set yrange [" << ymin_ << ":" << ymax_ << "]" << std::endl;
    }   
    if(label_)
    {	
	skript << "set xlabel " << xlabel_ << std::endl;
	skript << "set ylabel " << ylabel_ << std::endl;
    }
    skript << "plot '" + pfad_ + name_ + ".dat' with lines" + " title '" +  title_ + "'" << std::endl;
    
    skript.close();
}

void plotter::writedaten_()
{
    double xwert = xmin_;
    double ywert;
    std::ofstream datei((pfad_ + name_ + ".dat").c_str());
    
    for (int i = 0; i < schritte_; i++)
    {  
	ywert = function_(xwert);
	datei << xwert << " " << ywert << std::endl;
	
	xwert += schrittweite_;
    }
    
    datei.close();
}

int plotter::plot(std::function<double(double)> function)
{
    function_ = function;
    
    writeskript_();
    writedaten_();

    system(("gnuplot '" + pfad_ + "skript_" + name_ + ".plt" + "'").c_str()); 
    
    return 0;
}

int plotter::plot(std::function<double(double)> function, std::string pfad)
{
  setpfad(pfad);
  return plot(function);
}

int plotter::plot(std::function<double(double)> function, std::string pfad, std::string name)
{
  setpfad(pfad);
  setname(name);
  return plot(function);
}

int plotter::plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte)
{
  setpfad(pfad);
  setname(name);
  setschritte(schritte);
  return plot(function);
}

int plotter::plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop)
{
  setpfad(pfad);
  setname(name);
  setschritte(schritte);
  setxrange(start,stop);
  return plot(function);
}

int plotter::plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop, std::string xlabel, std::string ylabel)
{
  setpfad(pfad);
  setname(name);
  setschritte(schritte);
  setxrange(start,stop);
  setxlabel(xlabel);
  setylabel(ylabel);
  return plot(function);
}


std::string plotter::plot3d(std::function<double(double,double)> function, std::string pfad, std::string name, double schritte, double start, double stop)
{
  return "";
}