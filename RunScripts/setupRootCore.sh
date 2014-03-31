#!/bin/bash

cd ../

cd RootCore
#./configure
#./configure --enable-pedantic=force CXXFLAGS="-D__USE_XOPEN2K8"
./ configure --enable-pedantic=force CXXFLAGS="-g -Wall"
cd ../

source RootCore/scripts/setup.sh

cd RunScripts

