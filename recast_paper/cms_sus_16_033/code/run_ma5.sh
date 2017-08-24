#!/usr/bin/env zsh

ma5dir=/home/jory/bin/maonepointsixbeta/v1.6beta/ra2b/
# ma5dir=/home/jory/bin/madanalysis5/ra2b/
inputdir=${ma5dir}Input

curdir=$PWD
cd ${ma5dir}Build
source setup.sh
for modelpoint in $(ls $inputdir/SMS*)
    do
        ./MadAnalysis5job $modelpoint
    done
cd $curdir


