#!/usr/bin/env zsh

ma5dir=/home/jory/bin/maonepointsixbeta/v1.6beta/ra2b/
# ma5dir=/home/jory/bin/madanalysis5/ra2b/
#rootfiledir=/home/jory/ma5recasts/federico/Root_Files/
rootfiledir=~/ma5recastsfiles/cms_sus_16_033/

for filename in $(ls ${rootfiledir}*root)
    do
        filename_noextension=$filename[0,-6]
        filename_short=$(basename ${filename_noextension})
        echo $(realpath $filename) '>'  ${ma5dir}Input/$filename_short
        echo $(realpath $filename) > ${ma5dir}Input/$filename_short
    done