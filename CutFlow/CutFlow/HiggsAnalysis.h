#ifndef HIGGANALYSIS_H
#define HIGGANALYSIS_H

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
		HiggsAnalysis(TTree *initPhysicsTree);
		~HiggsAnalysis();
		
		void AnalyzeTree();
		
		void InitializeVar();
		
		void SetOutputFilePath(string newFilePath);		
	
	private:	
		TTree *m_physicsTree;
		
		D3PDReader::Event *m_event;
		
		TString m_outputFilePath;


#endif