#set term tikz size 6.0cm, 5.0cm fontscale 0.5 dl 0.5 fulldoc
set terminal pngcairo
set decimalsign ","
unset xtics
unset ytics
unset colorbox
set xrange [0:15]
set yrange [0:15]
set cbrange [-1:1]
set cbtics offset -0.8, 0.0 ("baixo" -1, "cima" 1)
set border
set size ratio -1
set nokey
set palette defined ( -1 '#B0C4DE', 1 '#DC143C')
set output "t-0.png"
plot "Temperatura-0.dat" u ($1):($2):($3) matrix w image t''

#f=10
#while(f <= 39){
#	set output sprintf("t-%d.png", f)
#	plot sprintf("Temperatura-%d.dat", f) u ($1):($2):3 matrix w image t''
#	unset output
#	f=f+1
#}
