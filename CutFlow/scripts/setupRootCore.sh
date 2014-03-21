cd ../

cd RootCore
./configure --enable-pedantic=force CXXFLAGS="-D__USE_XOPEN2K8"
cd ..

source RootCore/scripts/setup.sh

cd CutFlow

