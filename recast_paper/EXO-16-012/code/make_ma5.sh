#!/usr/bin/env zsh

ma5dir=/home/jory/bin/maonepointsixbeta/v1.6beta/ra2b/
# ma5dir=/home/jory/bin/madanalysis5/ra2b/

curdir=$PWD
cd ${ma5dir}Build
source setup.sh
cd ${ma5dir}Build/SampleAnalyzer/User/Analyzer
cp /home/jory/ma5recasts/cms_sus_16_033/code/cms_sus_16_033.* .
cd ${ma5dir}Build
make
cd $curdir
