#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <TString.h>
#include <TChain.h>
#include <TROOT.h>

using namespace std;
int main(int argc, char *argv[])
{
	gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");
	
	ifstream dataFileName;
	dataFileName.open("DataFiles/DataFileMC2012_NewGeo_Chain.txt");
	
	TString filePath;
	TChain *phyData = new TChain ("physics");
	do{
 	  	dataFileName >> filePath;
 		if (filePath.Length() <= 0) continue;
 		cout << filePath << endl;	
		phyData->Add(filePath.Data());
	} while (dataFileName.good());
	
	HiggsAnalysis *cutFlow = new HiggsAnalysis(phyData);
	
	cutFlow->InitializeVar();
	cutFlow->SetupPrintEventList(fileOverWrite, "EventList");
	cutFlow->SetOutputFilePath("Output/mc12c_VBFH125.root");
	
	int passedAllCut = 0;
	passedAllCut = cutFlow->AnalyzeTree();
	
	delete cutFlow;
	delete phyData;
}