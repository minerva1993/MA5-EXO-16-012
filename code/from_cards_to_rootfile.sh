#!/usr/bin/env zsh
# cc0 copyleft -- public domain

mg5dir=
delphesdir=
outputfiledir=


if [[ -n $outputfiledir ]]
    then
        echo using outputfiledir $outputfiledir
    else
        outputfiledir=/data/bin/korea/outputfiles
        echo using outputfiledir $outputfiledir
fi

if [[ -n $mg5dir ]]
    then
        echo using mg5dir $mg5dir
    else
        mg5dir=/data/bin/korea/MG5_aMC_v2_6_0
        echo using mg5dir $mg5dir
fi

if [[ -n $delphesdir ]]
    then
        echo using mg5dir $delphesdir
    else
        delphesdir=/data/bin/delphes
        echo using mg5dir $delphesdir
fi



# Gather any command line arguments
if [[ -n $1 ]]
    then
        proccard=$(realpath $1)
        echo Using proccard $proccard
    else
        echo Usage:
        echo ./from_cards_to_rootfile proccard runcard pythiacard slha delphescard
        exit
fi
if [[ -n $2 ]]
    then
        runcard=$(realpath $2)
        echo Using runcard $runcard
    else
        echo Using default run card but you can also use:
        echo ./from_cards_to_rootfile proccard runcard
fi
if [[ -n $3 ]]
    then
        pythiacard=$(realpath $3)
        echo Using pythiacard $pythiacard
    else
        echo Using default pythia card but you can also use:
        echo ./from_cards_to_rootfile proccard runcard pythiacard
fi
if [[ -n $4 ]]
    then
        slha=$(realpath $4)
        echo Using slha $slha
    else
        echo Using default param_card but you can also use:
        echo ./from_cards_to_rootfile proccard runcard pythiacard slha
fi
if [[ -n $5 ]]
    then
        delphescard=$(realpath $5)
        echo Using delphescard $delphescard
    else
        echo Using default delphes CMS card but you can also use:
        echo ./from_cards_to_rootfile proccard runcard pythiacard slha delphescard
        delphescard=${delphesdir}/cards/delphes_card_CMS.tcl
fi

curdir=$PWD



# MadGraph
cd $mg5dir

# Get mg5 output dir from proccard:
for a in $(grep output $proccard)
do
    if [ ${a} != '-nojpeg' ]
    then
        prodname=$a
    fi
done

echo Using process directory name $prodname as read from proccard $proccard
echo ''
echo '******************************************************************'
echo ''

##### Generate MadGraph dir ####
./bin/mg5_aMC < ${proccard}
echo ''
echo '******************************************************************'
echo ''
echo Done creating madgraph process with $proccard , results can be found in $prodname
echo ''
echo '******************************************************************'
echo ''

##### Generate events and shower
cd $prodname
if [[ -n $slha ]]
then
    cp $slha ${mg5dir}/${prodname}/Cards/param_card.dat
fi
if [[ -n $runcard ]]
then
    cp $runcard ${mg5dir}/${prodname}/Cards/run_card.dat
fi
if [[ -n $pythiacard ]]
then
    cp $pythiacard ${mg5dir}/${prodname}/Cards/pythia8_card.dat
fi
run_name=${prodname}
##### Go to MadGraph dir ####
cd ${mg5dir}/${prodname}

# Make sure to run with pythia8:
echo shower > instructions
# Run to generate events:
./bin/generate_events $run_name --multicore < instructions

echo ''
echo '******************************************************************'
echo ''
echo Done running madgraph -- you can find the results in ${mg5dir}/Events${run_name}
echo ''
echo '******************************************************************'
echo ''

# Move pythia file
pythiafilename=${outputfiledir}${run_name}.hepmc.gz
cp Events/${run_name}/tag_1_pythia8_events.hepmc.gz ${pythiafilename}

# Unzip pythiafile
pythia_unzipped=$pythiafilename[0,-4]
if [[ -a $pythia_unzipped ]]
then
    echo ''
    echo '******************************************************************'
    echo ''
    echo $pythia_unzipped exists! Moving it to ${pythia_unzipped}.bak
    echo ''
    echo '******************************************************************'
    echo ''
    mv $pythia_unzipped ${pythia_unzipped}.bak
fi
gunzip ${pythiafilename}


echo ''
echo '******************************************************************'
echo ''
echo Done untarring pythia file -- you can find the pythia file in $pythia_unzipped
echo ''
echo '******************************************************************'
echo ''

# Delphes
cd $delphesdir
rootfile=${outputfiledir}${run_name}.root
if [[ -a $rootfile ]]
then
    echo ''
    echo '******************************************************************'
    echo ''
    echo $rootfile exists! Moving it to ${rootfile}.bak
    echo ''
    echo '******************************************************************'
    echo ''
    mv $rootfile ${rootfile}.bak
fi
# Run Delphes
./DelphesHepMC $delphescard $rootfile $pythia_unzipped

echo ''
echo '******************************************************************'
echo ''
echo Done running delphes -- you can find the root file in $rootfile
echo ''
echo '******************************************************************'
echo ''

cd $curdir

