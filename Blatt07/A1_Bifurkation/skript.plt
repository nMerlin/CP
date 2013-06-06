reset
set term png
set output "A1_LogistischeAbbildung.png"
set yrange [0:1.1]
plot "LogAbb.dat" using 1:2 with points pt 0
reset
set term png
set output "A1_KubischeAbbildung.png"
plot "KubAbb.dat" using 1:2 with points pt 0