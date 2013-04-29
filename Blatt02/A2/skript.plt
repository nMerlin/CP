set terminal pdf
set output './Iqxqy.pdf'
set xrange [-6:6]
set yrange [-6:6]
splot './Iqxqy.dat' with lines title 'Iqxqy'
