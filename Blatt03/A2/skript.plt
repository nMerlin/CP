set terminal pdf
set output './harm_oszi.pdf'
plot './harm_oszi.dat' with lines title 'harm_oszi', cos(x)
