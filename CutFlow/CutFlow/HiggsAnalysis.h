#ifndef HIGGANALYSIS_H
#define HIGGANALYSIS_H

#include "../../D3PDReader/D3PDReader/Event.h"

#include <vector>
#include <string>
#include "math.h"

#include <TTree.h>
#include <TChain.h>
#include <TString.h>

using namespace std;
class HiggsAnalysis
{
	public:
		HiggsAnalysis();
		HiggsAnalysis(TTree *tPhysicsTree);
		~HiggsAnalysis();
		
		void AnalyzeTree();
		void AnalyzeTreeEvent(Long64_t eventNumber);
		
		void InitializeVar();
		
		void SetOutputFilePath(string newFilePath);		
	
	private:	
		TTree *m_physicsTree;
		
		D3PDReader::Event *m_event;
		
		TString m_outputFilePath;

		TString m_currFileName;

};

#endif