mkdir Plot
mkdir cgPlot

while true
do
  python3 plot.py Log Plot 
  python3 plot2.py cgLog cgPlot
  python3 plot.py cgLog cgPlot
  python3 plot2_gaussian.py Log Plot 
  sleep 10
done