#ifndef HIGGSANALYSIS_H
#define HIGGSANALYSIS_H

#include "D3PDReader/Event.h"
#include "PileupReweighting/TPileupReweighting.h"

#include "CutFlow4Lep/ParticleObjects/Jets.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

#include "CutFlow4Lep/Weights/EventWeight.h"
#include "CutFlow4Lep/Weights/ggFWeight.h"
#include "CutFlow4Lep/Weights/JHUWeight.h"
#include "CutFlow4Lep/Weights/PileupWeight.h"
#include "CutFlow4Lep/Weights/ZVertexWeight.h"

#include "CutFlow4Lep/Triggers/ElectronTrigger.h"
#include "CutFlow4Lep/Triggers/MuonTrigger.h"
#include "CutFlow4Lep/Triggers/DiElectronTrigger.h"
#include "CutFlow4Lep/Triggers/DiMuonTrigger.h"
#include "CutFlow4Lep/Triggers/ElectronMuonTrigger.h"

#include "CutFlow4Lep/Smear/D0Z0Smear.h"
#include "CutFlow4Lep/Smear/MuonCaloSmear.h"
#include "CutFlow4Lep/Smear/MuonStacoSmear.h"
#include "CutFlow4Lep/Smear/ElectronSmear.h"
#include "CutFlow4Lep/Smear/PhotonSmear.h"

#include "CutFlow4Lep/Corrections/JetCalibration.h"

#include "CutFlow4Lep/Cuts/DataPreselection.h"
#include "CutFlow4Lep/Cuts/VertexCut.h"
#include "CutFlow4Lep/Cuts/MuonCut.h"
#include "CutFlow4Lep/Cuts/ElectronCut.h"

#include "CutFlow4Lep/Overlap/MuonOverlap.h"
#include "CutFlow4Lep/Overlap/ElectronOverlap.h"

#include "CutFlow4Lep/Calculations/MCHiggsMass.h"
#include "CutFlow4Lep/Output/OutputTree.h"
#include "CutFlow4Lep/StructDef.h"
#include "CutFlow4Lep/PileupReweightTool.h"
#include "CutFlow4Lep/UsesEvent.h"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <TH1F.h>
#include <TFile.h>

using namespace std;
class HiggsAnalysis :public UsesEvent
{
	public:
		HiggsAnalysis();
		HiggsAnalysis(TChain *tPhysicsTree);
		~HiggsAnalysis();
		
		void analyzeTree();
		void analyzeTreeEvent(Long64_t eventNumber);

		void setMCCollection();
		void setDataCalibration();
		void setEventYear();
		void setSampleType();
		void setCurrFileNameVec();
		void setMCRunNumber();
		void setDataPeriod();
		void setIsTauSampleAndMCGenerator();
		void setRunNumberSfAndLbnSf(Root::TPileupReweighting *pileupReweightingTool);
		void setCurrElectron();
		void setMCChannelNumber();
		
		void setOutputFilePath(string newFilePath);	

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

	private:
		// Constructor
		TTree *m_physicsTree;
		D3PDReader::Event *m_event;
		TString m_outputFilePath;
		TString m_currFileName;
		vector<TString> m_currFileNameVec;
		TH1F *m_countingHist;

		// Variables describing each event
		Bool_t m_isMC;
		Bool_t m_isTauSample;
		Bool_t m_is2012;

		Int_t m_mcChannelNumber;
		Int_t m_dataYear;
		Int_t m_mcGenerator;
		Int_t m_currMCCollection;
		Int_t m_currDataCalibration;
		Int_t m_sampleType;
		Int_t m_dataPeriod;
		Int_t m_runNumber_sf;
		Int_t m_lbn_sf;

		Double_t m_cmEnergy;

		D3PDReader::ElectronD3PDObject *m_currElectron;

		Long64_t m_mcRunNumber;

};

#endif