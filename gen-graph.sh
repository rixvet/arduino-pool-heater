#!/bin/sh
#
# Based on http://oss.oetiker.ch/rrdtool/tut/rrdtutorial.en.html
#
# Process the raw sensor data from an Arduino temperature monitoring
#
# Rick van der Zwet <info@rickvanderzwet.nl>
#

RRDFILE=test.rrd
OUTPUT=pool.png
FAILSAFE=5

if [ -z "$1" ]; then
  echo "Usage: $0 <input1.txt> [<input2> ...]" 1>&2
  exit 128
fi

if [ -r "$RRDFILE" ]; then
  echo "# Start fresh every time, deleting $RRDFILE. Will continue in $FAILSAFE seconds, press CTRL+C to cancel" 1>&2
  sleep $FAILSAFE
  rm $RRDFILE
fi

# Store a week worth of data
rrdtool create $RRDFILE           \
         --start 920804400        \
         --step 1                 \
         DS:sensor1:GAUGE:1:0:100 \
         DS:sensor2:GAUGE:1:0:100 \
         DS:sensor3:GAUGE:1:0:100 \
         DS:sensor4:GAUGE:1:0:100 \
         DS:speed:GAUGE:1:0:100   \
         RRA:AVERAGE:0.5:1:604800 \
         RRA:AVERAGE:0.5:60:10080 \
         RRA:AVERAGE:0.5:3600:168   

`dirname $0`/raw-data-parser.py $@ | xargs rrdtool update $RRDFILE || exit 1

# Nicely output
rrdtool graph $OUTPUT                                \
   --width 1200 --height 600 --step 1                \
   --start 920804400 --end ` rrdtool last $RRDFILE`  \
   --vertical-label "Temperatuur in graden Celsius"                          \
   --title "5m^3 zwembad opgewarmd door een zonnecollector"                  \
   --x-grid none \
   DEF:sensor1=$RRDFILE:sensor1:AVERAGE LINE:sensor1#FF0000:"Collector outlet"\
   DEF:sensor2=$RRDFILE:sensor2:AVERAGE LINE:sensor2#00FF00:"Collector inlet"\
   DEF:sensor3=$RRDFILE:sensor3:AVERAGE LINE:sensor3#0000FF:"Zwembad water"\
   DEF:sensor4=$RRDFILE:sensor4:AVERAGE LINE:sensor4#00FFFF:"Buitenlucht" \
   DEF:speed=$RRDFILE:speed:AVERAGE LINE:speed#000000:"Pomp Snelheid in Procent"

echo "Data stored at: $RRDFILE" 
echo "Picture stored: $OUTPUT"
