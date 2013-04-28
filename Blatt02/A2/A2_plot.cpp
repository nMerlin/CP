#include "A2_plot.h"

//Erstellt alle Plots (Energie, Drehmoment als Ableitung, Drehmoment klassisch), später vielleicht in eine vernünftige Klasse umschreiben, da die Funktionalität garantiert wiederverwendet wird
std::string plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop) {
	
	//***Variablen und Konstanten***
	double h = (stop - start)/schritte;
	double x = start;
	
	//Dateinamen und Titel im Plot
	std::string gp_skript = "skript.plt";
	std::string gp_title = name;
	std::string name_plot = name;

	//***Dateistreams öffnen***
	//Ableitung der Energie
	std::ofstream datei((pfad + name + ".dat").c_str()); //*.c_str() wandelt einen String in einen char-String um
	//ableitung << beschriftung << std::endl;	//Titelzeile
	
	//***Gnuplot-Skript***
	std::ofstream skript((pfad + gp_skript).c_str());
	skript << "set terminal pdf" << std::endl;
	skript << "set output '" + pfad + name_plot + ".pdf'" << std::endl;
	skript << "set xrange [" << start << ":" << stop << "]" << std::endl;
	//skript << "set xlabel 'Theta_0 [rad]'" << std::endl;
	//skript << "set ylabel 'Energie [willk. Einh.]'" << std::endl;
	skript << "plot '" + pfad + name_plot + ".dat' with lines" + " title '" +  gp_title + "'" << std::endl;

	//Daten in die Gnuplot-Datenquelle einfüllen
	
	for (int i = 0; i < schritte; i++) {
		
		double y = function(x);
		
		//Daten in Datei schreiben
		datei
		<< x
		<< " "
		<< y
		<< std::endl;
		
		x += h;
	}
	
	//Damit Gnuplot arbeiten kann, müssen die Streams erst geschlossen werden
	datei.close();
	skript.close();
	
	//Aufruf von Gnuplot und Übergabe des soeben erstellten Skriptes, welches drei Plots erstellt
	system(("gnuplot '" + pfad + gp_skript + "'").c_str());
	
	return pfad;
}