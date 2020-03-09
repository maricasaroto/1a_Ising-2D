set term tikz size 6.0cm, 5.0cm fontscale 0.5 dl 0.5 fulldoc
set output "E-N-tex.tex"
set decimalsign ","
set grid 
set tics scale 0.5
set tics nomirror
set xtics  offset 0.0, 0.0
set ytics  offset 1.0, 0.0

set grid
set title "Modelo de Ising"
set xlabel "Temperatura ($T$)"
set ylabel "Energia por spin ($E/L$)"
set key at graph 0.30, 0.97

plot "data/2.dat" u 1:2 w lp pt 7 lw 1 ps 0.3 lc 6 t'$L=02$',"data/4.dat" u 1:2 w lp pt 7 lw 1 ps 0.3 lc 2 t'$L=04$',"data/8.dat" u 1:2 w lp pt 7 lw 1 ps 0.3 lc 7 t'$L=08$',"data/16.dat" u 1:2 w lp pt 7 lw 1 ps 0.3 lc 4 t'$L=16$'


unset output
