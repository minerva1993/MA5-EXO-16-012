#!/usr/bin/env zsh
# cc0 copyleft -- public domain

# Gather any command line arguments
if [[ -n $1 ]]
    then
        prodname=$1
    else
        echo Usage:
        echo ./from_cards_to_rootfile productionname modelname
        echo Production names: SMS-GlGl_onejet SMS-SqSq_onejet
        echo Model names: T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800
        exit
fi
if [[ -n $2 ]]
    then
        modelname=$2
    else
        echo Usage:
        echo ./from_cards_to_rootfile productionname modelname
        echo Production names: SMS-GlGl_onejet SMS-SqSq_onejet
        echo Model names: T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800
        exit
fi
if [[ -n $bindir ]]
    then
        echo using bindir $bindir
    else
        bindir=~/bin
fi
if [[ -n $repodir ]]
    then
        echo using repodir $repodir
    else
        repodir=~/ma5recasts
fi
if [[ -n $repodir ]]
    then
        echo using repodir $repodir
    else
        repodir=~/ma5recasts
fi
if [[ -n $outputfiledir ]]
    then
        echo using outputfiledir $outputfiledir
    else
        outputfiledir=/data/federico/
fi
 
 
if [[ -n $mg5dir ]]
    then
        echo using mg5dir $mg5dir
    else
        mg5dir=${bindir}/MG5_aMC_v2_5_2
fi



curdir=$PWD

ma5carddir=${repodir}/cms_sus_16_033/cards/mgpyth8/
delphesdir=${bindir}/delphes/
#delphescard=~/ma5recasts/cms_sus_16_033/cards/delphes_skimmed_CMS_13TeV.tcl
delphescard=${repodir}/cms_sus_16_033/cards/delphes_card_CMS.tcl
#~/ma5recasts/cms_sus_16_033/cards/delphes_card_CMS_SUSY.tcl


# MadGraph
cd $mg5dir

##### Generate MadGraph dir ####
./bin/mg5_aMC < ${ma5carddir}${prodname}.dat
echo Done creating madgraph process -- you can find the process dir under ${mg5dir}${prodname}

##### Generate events and shower
cd ${mg5dir}/${prod_name}
cp ${ma5carddir}${modelname}.dat ${mg5dir}/${prodname}/Cards/param_card.dat
cp ${ma5carddir}${prodname}_run_card.dat ${mg5dir}/${prodname}/Cards/run_card.dat
cp ${ma5carddir}${prodname}_${modelname}_pythia8_card.dat ${mg5dir}/${prodname}/Cards/pythia8_card.dat
run_name=${prodname}_${modelname}
##### Go to MadGraph dir ####
cd ${mg5dir}/${prodname}

# Make sure to run with pythia8:
echo shower > instructions
# Run to generate events:
./bin/generate_events $run_name --multicore -f < instructions

echo Done running madgraph -- you can find the results in ${mg5dir}/Events${run_name}

# Move pythia file
pythiafilename=${outputfiledir}${run_name}.hepmc.gz
cp Events/${run_name}/tag_1_pythia8_events.hepmc.gz ${pythiafilename}

# Unzip pythiafile
gunzip ${pythiafilename}

echo Done untarring pythia file -- you can find the pythia file in $pythiafilename

# Delphes
cd $delphesdir
rootfile=${outputfiledir}${run_name}.root
if [[ -a $rootfile ]]
then
    echo $rootfile exists! Moving it to ${rootfile}.bak
    mv $rootfile ${rootfile}.bak
fi
# Run Delphes
./DelphesHepMC $delphescard $rootfile $pythiafilename[0,-4]

echo Done running delphes -- you can find the root file in $rootfile

cd $curdir

