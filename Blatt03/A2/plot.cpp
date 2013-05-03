#include "plot.hpp"

//Plottet die gegebene Funktion von start bis stop in schritte schritten in 2D
std::string plot(std::function<double(double)> function, std::string pfad, std::string name, double schritte, double start, double stop) {
	
	//***Variablen und Konstanten***
	double h = (stop - start)/schritte;
	double x = start;
	
	//Dateinamen und Titel im Plot
	std::string gp_skript = "skript.plt";
	std::string gp_title = name;
	std::string name_plot = name;

	//***Dateistreams öffnen***
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

//Plottet die gegebene Datei in 2D
std::string plot(std::string pfad, std::string name) {
	//Dateinamen und Titel im Plot
	std::string gp_skript = "skript.plt";
	std::string gp_title = name;
	std::string name_plot = name;

	//***Gnuplot-Skript***
	std::ofstream skript((pfad + gp_skript).c_str());
	skript << "set terminal pdf" << std::endl;
	skript << "set output '" + pfad + name_plot + ".pdf'" << std::endl;
	//skript << "set xrange [" << start << ":" << stop << "]" << std::endl;
	//skript << "set yrange [" << start << ":" << stop << "]" << std::endl;
	//skript << "set xlabel 'Theta_0 [rad]'" << std::endl;
	//skript << "set ylabel 'Energie [willk. Einh.]'" << std::endl;
	skript << "plot '" + pfad + name_plot + ".dat' with lines" + " title '" +  gp_title + "'" << std::endl;

	skript.close();

	//Aufruf von Gnuplot und Übergabe des soeben erstellten Skriptes, welches drei Plots erstellt
	system(("gnuplot '" + pfad + gp_skript + "'").c_str());
	
	return pfad;	
}

//Plottet die gegebene Funktion (3D-Plot, symmetrisch in x und y)
std::string plot3d(std::function<double(double,double)> function, std::string pfad, std::string name, double schritte, double start, double stop) {
	//***Variablen und Konstanten***
	double h = (stop - start)/schritte;
	
	//Dateinamen und Titel im Plot
	std::string gp_skript = "skript.plt";
	std::string gp_title = name;
	std::string name_plot = name;

	//***Dateistreams öffnen***
	std::ofstream datei((pfad + name + ".dat").c_str()); //*.c_str() wandelt einen String in einen char-String um
	
	//***Gnuplot-Skript***
	std::ofstream skript((pfad + gp_skript).c_str());
	skript << "set terminal pdf" << std::endl;
	skript << "set output '" + pfad + name_plot + ".pdf'" << std::endl;
	skript << "set xrange [" << start << ":" << stop << "]" << std::endl;
	skript << "set yrange [" << start << ":" << stop << "]" << std::endl;
	//skript << "set xlabel 'Theta_0 [rad]'" << std::endl;
	//skript << "set ylabel 'Energie [willk. Einh.]'" << std::endl;
	skript << "splot '" + pfad + name_plot + ".dat' with lines" + " title '" +  gp_title + "'" << std::endl;
	
	static int s = schritte;
	
	//Daten in die Gnuplot-Datenquelle einfüllen
	//Dateibefehle außerhalb der Schleifen
	//pragma liefert unsortierte Datenpunkte in der Datei
	//#pragma omp parallel for
	for (int j = 0; j < s; j++) {
		double y = start + j*h;
		
		for (int i = 0; i < s; i++) {
			
			double x = start + i*h;
			double z = function(x,y);
			
			//Daten in Datei schreiben
			datei
			<< x
			<< " "
			<< y
			<< " "
			<< z
			<< std::endl;
			
		}
	}
	
	//Damit Gnuplot arbeiten kann, müssen die Streams erst geschlossen werden
	datei.close();
	skript.close();
	
	//Aufruf von Gnuplot und Übergabe des soeben erstellten Skriptes, welches drei Plots erstellt
	system(("gnuplot '" + pfad + gp_skript + "'").c_str());
	
	return pfad;
}