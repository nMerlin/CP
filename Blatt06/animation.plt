#We will plot an md-simulation in this gif animation
reset
set term gif animate
set output "harmOszi.gif"
n=100    #n frames
set yrange [0:0.06]
i=0
load "animate.gnuplot"