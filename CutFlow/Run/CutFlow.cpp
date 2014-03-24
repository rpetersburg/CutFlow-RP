// CutFlow.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <TString.h>
#include <TChain.h>
#include <TROOT.h>

#include "../CutFlow/HiggsAnalysis.h"

using namespace std;
void CutFlow()
{
//int main(int argc, _TCHAR* argv[])
//{
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
	
	// Instantiate the primary analysis object
	HiggsAnalysis *cutFlow = new HiggsAnalysis(phyData);
	
	cutFlow->InitializeVar();
	cutFlow->SetOutputFilePath("Output/mc12c_VBFH125.root");
	
	// Run the cut flow
	cutFlow->AnalyzeTree();
	
	// Delete all instantiated objects for clean exit
	delete cutFlow;
	delete phyData;
	//return 0;
}