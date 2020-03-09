set term tikz size 6.0cm, 5.0cm fontscale 0.5 dl 0.5 fulldoc
set output "mc-tex.tex"
#set terminal pngcairo
#set output "m-c.png"
set grid
set xtics  offset 0.0, 0.0
set ytics  offset 1.0, 0.0
set tics nomirror
set tics scale 0.5
set decimalsign ","
set xrange [2500:5500]
set yrange [-1.2:1.2]
#set ytics 1.0, 0.5, 0.0, -0.5, -1.0


set grid
set title "Modelo de Ising"
set xlabel "Passo de Monte Carlo ($MCS$)"
set ylabel "Magnetização por spin ($M/L$)"
#set key at graph 0.30, 0.97

plot "data/monte-carlo.dat" u 1:2 w l lw 1 lc 6 t' $T=1,0$  e $L=2$'


unset output
