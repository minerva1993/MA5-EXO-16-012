#!/usr/bin/env zsh

name=ra2b
analysis=cms_sus_16_033
ma5dir=/home/jory/bin/maonepointsixbeta/v1.6beta/
curdir=$PWD
cd ${ma5dir}Build
echo $name > tmpfile
echo $analysis >> tmfile
cat tmpfile | ./bin/ma5 -R -E
ls ${name}/Build/SampleAnalyzer/User/Analyzer/
cd $curdir

