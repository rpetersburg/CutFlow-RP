#source computerSetup.sh
#source setupRootCore.sh

cd ../../

#$ROOTCOREDIR/scripts/find_packages.sh
#$ROOTCOREDIR/scripts/compile.sh

cd CutFlow/src/main

root -l -b 'CutFlow.cxx'

cd ../../scripts/

