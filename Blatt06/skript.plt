reset
set term png
set output "A1c.png"
plot "A1c.dat" using 1:2 with lines
set output "A1d_Zustand.png"
plot "A1d_Zustand.dat" using 1:2 with lines
set output "A1d_Normierung.png"
plot "A1d_Normierung.dat" using 1:2 with lines
set output "A1f_Mittelwert_Ort.png"
plot "A1f.dat" using 1:2 with lines
set output "A1f_Schwankung_Ort.png"
plot "A1f.dat" using 1:3 with lines
set output "A1f_Mittelwert_Impuls"
plot "A1f.dat" using 1:4 with lines
set output "A1f_Schwankung_Impuls"
plot "A1f.dat" using 1:5 with lines