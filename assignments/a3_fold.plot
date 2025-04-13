set grid
set key outside
set datafile separator ",";

plot "out/folding_period.csv" using 1:2 with points title "Folding Period";

pause -1 "[ENTER] to continue"