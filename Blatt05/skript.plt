#We will plot an md-simulation in this gif animation
reset
set term gif animate
set output "md.gif"
n=100    #n frames
set xrange [0:8]
set yrange [0:8]
i=0
load "animate.gnuplot"