#!/usr/bin/env python

from operator import itemgetter
import sys

total_delay = 0

current_year = None
current_origin = None
current_delay = 0
current_flights = 0
year = None
origin = None
flights = None
total = 0
# input comes from STDIN
for line in sys.stdin:
	# remove leading and trailing whitespace
	line = line.strip()

	# parse the input we got from mapper.py
	key, delay = line.split('\t')
	year, origin = key.split(',')

	# convert count (currently a string) to int
	try:
		delay = int(delay)
	except ValueError:
		# count was not a number, so silently
		# ignore/discard this line
		continue

	# this IF-switch only works because Hadoop sorts map output
	# by key (here: word) before it is passed to the reducer
	if current_year == year and current_origin == origin:
		current_delay += delay
		total_delay += delay
		current_flights += 1
		
	else:
		if current_year and current_origin:
			# write result to STDOUT
			if(current_origin == "ATL" and current_year == "2007"):
			  print '%s\t%s\t%0.2f'% (current_year, current_origin, float(current_delay)/float(current_flights))

		current_delay = delay
		current_year = year
		current_origin = origin
		current_flights = 1

# do not forget to output the last word if needed!
if current_year == year and current_origin == origin:
	if(current_origin == "ATL" and current_year == "2007"):
	  print '%s\t%s\t%0.2f'% (current_year, current_origin, float(current_delay)/float(current_flights))

	