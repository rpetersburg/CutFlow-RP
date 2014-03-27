#ifndef HIGGSANALYSIS_H
#define HIGGSANALYSIS_H

#include "D3PDReader/Event.h"
#include "CutFlow4Lep/ParticleObjects/Jets.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"
#include "CutFlow4Lep/Output/OutputTree.h"

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
		HiggsAnalysis(TTree *tPhysicsTree);
		~HiggsAnalysis();
		
		//void AnalyzeTree();
		//void AnalyzeTreeEvent(Long64_t eventNumber);
		//
		//void InitializeVar();
		//
		//void SetOutputFilePath(string newFilePath);	

		//Muon *muon;
		//Electron *electron;
		//Electron *electronLoose;
		//Jets *jets;
		//Jets *jetsTruth;
		//Jets *jets_Fid;
		//Jets *jetsTruth_Fid;

		//QuadLepton *higgsCandidate4Mu;
		//QuadLepton *higgsCandidate4El;
		//QuadLepton *higgsCandidate2L2L;

		//OutputTree *outputTree;
		//OutputTree *outputTreeCR;
		//OutputTree *outputTreelleeCR;

		//// Variables that store the final physics objects
		//vector<ChargedLepton *> muEvent;
		//vector<ChargedLepton *> elEvent;
		//vector<ChargedLepton *> elLooseEvent;
		//vector<ChargedLepton *> jetsEvent;
		//vector<ChargedLepton *> jetsTruthEvent;
		//vector<ChargedLepton *> jetsEvent_Fid;
		//vector<ChargedLepton *> jetsTruthEvent_Fid;
	
		//// Physics objects before final overlap removal
		//vector<ChargedLepton *> muOverlap;
		//vector<ChargedLepton *> elOverlap;
		//vector<ChargedLepton *> elLooseOverlap;
		//vector<ChargedLepton *> jetsOverlap;
		//vector<ChargedLepton *> jetsOverlap_Fid;

	
	//private:	
	//	TTree *m_physicsTree;
	//	
	//	D3PDReader::Event *m_event;
	//	
	//	TString m_outputFilePath;

	//	TString m_currFileName;

	//	TString outputFilePath;

};

#endif