set terminal pdf
set output '/Users/johannesthewes/Git/CP/Blatt02/A2/Iqxqy.pdf'
set xrange [-6:6]
set yrange [-6:6]
splot '/Users/johannesthewes/Git/CP/Blatt02/A2/Iqxqy.dat' with lines title 'Iqxqy'
