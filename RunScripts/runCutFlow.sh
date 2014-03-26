#!/bin/bash

cd ../

$ROOTCOREDIR/scripts/find_packages.sh
$ROOTCOREDIR/scripts/compile.sh

cd CutFlow/Run

root -l -b 'CutFlow.cxx'

cd ../../RunScripts
