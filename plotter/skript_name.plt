set grid
set terminal pdfcairo
set terminal pdfcairo enhanced
set output './name.pdf'
set xrange [0:6]
plot './name.dat' with lines title 'title'
