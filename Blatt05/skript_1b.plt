reset
set term png
set output "1b_v_Schwerpunkt.png"
plot "a1b.dat" using 1:2 with lines
set output "1b_Temperatur.png"
plot "a1b.dat" using 1:3 with lines
set output "1b_E_kin.png"
plot "a1b.dat" using 1:4 with lines
set output "1b_E_pot.png"
plot "a1b.dat" using 1:5 with lines