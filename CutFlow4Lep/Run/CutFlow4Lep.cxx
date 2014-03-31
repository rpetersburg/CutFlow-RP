#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <TString.h>
#include <TChain.h>
#include <TROOT.h>

//#include "../CutFlow4Lep/HiggsAnalysis.h"

using namespace std;
void CutFlow4Lep()
{
	// Load the libraries that ROOT core compiled...
	gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");
	
	// Open the relevant D3PD files contained in the specified .txt
	ifstream dataFileName;
	dataFileName.open("DataFiles/DataFileMC2012_NewGeo_Chain.txt");
	
	// Add files to the TChain phyData
	TString filePath;
	TChain *phyData = new TChain ("physics");
	do{
 	  	dataFileName >> filePath;
 		if (filePath.Length() <= 0) continue;
 		cout << filePath << endl;	
		phyData->Add(filePath.Data());
	} while (dataFileName.good());
	
	// Instantiate the primary analysis object and its variables
	HiggsAnalysis *cutFlow = new HiggsAnalysis(phyData);
	
	cutFlow->initializeVar();
	cutFlow->setOutputFilePath("Output/mc12c_VBFH125.root");
	
	// Run the cut flow
	cutFlow->analyzeTree();
	
	// Delete all instantiated objects for clean exit
	delete cutFlow;
	delete phyData;

}