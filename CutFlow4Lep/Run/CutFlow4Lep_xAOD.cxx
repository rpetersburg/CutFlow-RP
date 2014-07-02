#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <TString.h>
#include <TChain.h>
#include <TROOT.h>

//#include "../CutFlow4Lep/HiggsAnalysis.h"

using namespace std;
void CutFlow4Lep_xAOD()
{
	// Load the libraries that ROOT core compiled...
	gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");
	
	// Give the file for analysis
	TString dataFileName = "/afs/cern.ch/atlas/project/PAT/xAODs/r5534/valid2.117050.PowhegPythia_P2011C_ttbar.digit.AOD.e2657_s1933_s1964_r5534/AOD.01482225._000107.pool.root.1";

	// Instantiate the primary analysis object and its variables
	HiggsAnalysis *cutFlow = new HiggsAnalysis(dataFileName);
	
	//cutFlow->initializeVar();
	//cutFlow->setOutputFilePath("Output/mc12c_VBFH125.root");
	
	// Run the cut flow
	cutFlow->analyzeTree();
	
	// Delete all instantiated objects for clean exit
	delete cutFlow;
}