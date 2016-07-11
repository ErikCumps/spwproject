#!/bin/bash

./mtanal.pl -Si "$@" >REPORTSI
./mtanal.pl -Sa -csv "$@" >REPORTSA

