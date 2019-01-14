#!/bin/bash

(cat spwaw.set; cat winspww2.set) | sed -e 's/^[0-9][0-9]*\t//' | sort | uniq > unit_types.txt
cat spwaw.set | awk '{print $1 "\t" $2}' > spwaw_unit_map.txt
cat winspww2.set | awk '{print $1 "\t" $2}' > winspww2_unit_map.txt
