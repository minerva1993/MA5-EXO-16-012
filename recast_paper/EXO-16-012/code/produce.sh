#!/bin/bash

####! /bin/env zsh unknown


####! /bin/bash


printf "Start time: "; /bin/date 
printf "Job is running on node: "; /bin/hostname 
printf "Job running as user: "; /usr/bin/id 
printf "Job is running in directory: "; /bin/pwd 

# Testing and verbosity
testing=''
verbose=verbose
# uncomment this line below for testing:
# testing=testing


# Set directories
export fermioutdir=/store/user/jsonneve/forfederico/
export workdir=$PWD

# Gather command line argument
production=${1}
model=${2}



# cms software setup
# Setup CMSSW Base
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530

# Checkout recent cmssw version
if [ -r CMSSW_8_0_20/src ] ; then 
 echo release CMSSW_8_0_20 already exists
else
 scram p CMSSW CMSSW_8_0_20
fi
cd CMSSW_8_0_20/src
eval `scram runtime -sh`


# Get code and cards
git clone https://jsonneve@gitlab.cern.ch/jsonneve/ma5recasts.git

# Create bindir
mkdir bin
cd bin

# Get delphes
git clone https://github.com/delphes/delphes.git

# Get MadGraph
wget 'https://launchpad.net/mg5amcnlo/2.0/2.5.x/+download/MG5_aMC_v2.5.4.tar.gz'
# Unpack tar
tar xf MG5_aMC_v2.5.4.tar.gz
cd MG5_aMC_v2_5_4
# echo "install lhapdf6" > installma5.txt
echo "install pythia8" >> installma5.txt
./bin/mg5 < installma5.txt


# Set directories
cd ../../ma5recasts/cms_sus_16_033/code
mkdir ${workdir}/outputforfederico
export outputfiledir=${workdir}/outputforfederico
export bindir=${workdir}/bin
export repodir=${workdir}/ma5recasts
export mg5dir=${bindir}/MG5_aMC_v2_5_4

# Set filenames
echo "writing to:" $outputfiledir
echo "working on:" $production $model

# Print some info
echo 'this is your directory:' $PWD
echo 'this is what is in there:' $(ls)
echo
echo "Working hard..."

# Run madgraph, pythia, and delphes:
echo ./from_cards_to_rootfile.sh $production $model
./from_cards_to_rootfile.sh $production $model
# Copy files to outside cmssw dir just in case
echo cp $outputfiledir/* ${workdir}
cp $outputfiledir/* ${workdir}

run_name=${production}_${modelname}
rootfile=${outputfiledir}/${run_name}.root¬

# Copy output files to fermilab storage:
echo xrdcp $rootfile root://cmseos.fnal.gov/${fermioutdir}$testing${run_name}.root
xrdcp $rootfile root://cmseos.fnal.gov/${fermioutdir}$testing${run_name}.root

# Remove everything
cd $workdir
rm -rf CMSSW_8_0_20




echo "Science complete!"
