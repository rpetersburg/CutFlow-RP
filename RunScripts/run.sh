#!/bin/bash

cd ../

$ROOTCOREDIR/scripts/find_packages.sh
$ROOTCOREDIR/scripts/compile.sh

cd CutFlow4Lep/Run

root -l -b 'CutFlow4Lep.cxx'

cd ../../RunScripts
