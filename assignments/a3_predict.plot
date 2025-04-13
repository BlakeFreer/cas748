set ylabel "Values"
set grid
set key outside
set datafile separator ",";
set style line 2 lc rgb '#1f77b4' lt 1 lw 1 pt 7   # Blue
set style line 3 lc rgb '#ff7f0e' lt 1 lw 1 pt 7   # Orange
set style line 4 lc rgb '#2ca02c' lt 1 lw 1 pt 7   # Green
set style line 5 lc rgb '#d62728' lt 1 lw 1 pt 7   # Red
set style line 6 lc rgb '#000000' lt 1 lw 1 pt 7   # Black
set style line 7 lc rgb '#ff7f0e' lt 1 lw 2 pt 7   # Orange


stats 'out/prediction.csv' nooutput
plot for [i=2:STATS_columns] 'out/prediction.csv' using 1:i with lines ls i title columnheader(i);
pause -1 "[ENTER] to continue"

plot "out/autocorrelation.csv" using 1:2 with points title "Autocorrelation";
pause -1 "[ENTER] to continue"