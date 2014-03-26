#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>

#include <TTree.h>
#include <TChain.h>
#include <TString.h>

#include "CutFlow/HiggsAnalysis.h"

HiggsAnalysis::HiggsAnalysis(TTree *tPhysicsTree) : m_physicsTree(tPhysicsTree)
{
	m_event = new D3PDReader::Event();
	m_event->ReadFrom(m_physicsTree);	
	m_event->
}

HiggsAnalysis::~HiggsAnalysis()
{
	delete m_physicsTree;
	delete m_event;	
}

void HiggsAnalysis::AnalyzeTree()
{
	Int_t eventNum = 0;
	Int_t numPassed = 0;

	Int_t totNumEvents = m_physicsTree->GetEntries();

	for (Long64_t iEvent = 0; iEvent < totNumEvents; iEvent++)
	{
		Long64_t event = iEvent;
		TChain* chain = dynamic_cast<TChain*> (m_physicsTree);
		if (chain)
		{
			event = chain->LoadTree(iEvent);
		}
		// m_currFileName = chain->GetFile()->GetPath();
		AnalyzeTreeEvent(iEvent);
	}

	cout << "Tree was analyzed..." << endl;
}

void HiggsAnalysis::AnalyzeTreeEvent(Long64_t eventNumber)
{
	if(eventNumber >= 0) m_event->GetEntry(eventNumber);

	// Getting the initial event weight
	Double_t eventWeight = 1.0;

	cout << "Tree event was analyzed..." << endl;

	//// Initializing the event Specific Variables
	//InitializeEventVar();
	//getPeriodEvent();
	//Bool_t passCut = true;
	//
	//// Preselection cut for Data
	//if(!isMC)
	//{
	//	passCut = DataPreselectionCut();
	//	if(!passCut) return passCut;
	//}
	//cutPass[cutFlow::DataPreselection]++;
	//cutMuPass[cutMuFlow::DataPreselection] += (event->mu_staco.n() + event->mu_calo.n());
	//cutElPass[cutElFlow::DataPreselection] += (el_cur->n());
	//cutJetsPass[cutJetsFlow::DataPreselection] += event->jet_akt4topoem.n();
	//Hist->cutPassHistW->Fill(cutFlow::DataPreselection, eventWeight);	
	//
	////Rest of Preselection
	//passCut = AllPreselectionCut();
	//if(!passCut) return passCut;
	//cutPass[cutFlow::Preselection]++;
	//cutMuPass[cutMuFlow::Preselection] += (event->mu_staco.n() + event->mu_calo.n());
	//cutElPass[cutElFlow::Preselection] += (el_cur->n());
	//cutJetsPass[cutJetsFlow::Preselection] += event->jet_akt4topoem.n();
	//Hist->cutPassHistW->Fill(cutFlow::Preselection, eventWeight);

	//// Initial Trigger Cut
	//if(isMC)
	//{ 
	//	pileupTool->SetRandomSeed(314159+event->eventinfo.mc_channel_number()*2718+event->eventinfo.EventNumber());
	//	runNumber_sf = pileupTool->GetRandomRunNumber(event->eventinfo.RunNumber());
	//}
	//else runNumber_sf = event->eventinfo.RunNumber();
	//
	//// For Counting DiMuon Trigger
	//Bool_t passCut4Mu = SingleMuonTrigger(runNumber_sf) | DiMuonTrigger() ;
	//if(passCut4Mu) {cutPass[cutFlow::Trigger4Mu] ++; Hist->cutPassHistW->Fill(cutFlow::Trigger4Mu, eventWeight);}
	//
	//// for Counting electron trigger
	//Bool_t passCut4e = SingleElectronTrigger(runNumber_sf)| DiElectronTrigger(runNumber_sf);
	//if(passCut4e) {cutPass[cutFlow::Trigger4e] ++;; Hist->cutPassHistW->Fill(cutFlow::Trigger4e, eventWeight);}
	//
	//// If it passes any of the trigger, move ahead
	//passCut = SingleElectronTrigger(runNumber_sf)| DiElectronTrigger(runNumber_sf) | 
	//SingleMuonTrigger(runNumber_sf) | DiMuonTrigger() |
	//ElectronMuonTrigger();
	//
	//// For Trig eff studies
	//if(anaType == doAnalysis::trigeff4l) passCut = true;

	//if(!passCut) return passCut;
	//
	//cutPass[cutFlow::Trigger]++;
	//cutMuPass[cutMuFlow::Trigger] += (event->mu_staco.n() + event->mu_calo.n());
	//cutElPass[cutElFlow::Trigger] += (el_cur->n());
	//cutJetsPass[cutJetsFlow::Trigger] += event->jet_akt4topoem.n();
	//Hist->cutPassHistW->Fill(cutFlow::Trigger, eventWeight);

	//// Vars to store the efficiency
	//vector<Double_t> muonStacoEff;
	//vector<Double_t> muonCaloEff;

	//// Performs the Cuts on Muon..
	//// Clearing the vars
	//muonObj->clearVars();	
	//muonObj->SetHist(Hist);

	//// Clearing the corr
	//corr->ClearVars();
	//
	//fillMuonHelperVars();

	//// Smearing
	//if(isDebugCall) printMuonInfo();
	//if(isDebugCall) corr->debugCall();
	//if(doCorr && doSmearD0Z0 && isMC) corr->SmearD0Z0(&(event->mu_staco), event->eventinfo.EventNumber(), leptonType::MuonStaco);
	//if(doCorr && doSmearD0Z0 && isMC) corr->SmearD0Z0(&(event->mu_calo), event->eventinfo.EventNumber(), leptonType::MuonCalo);
	//if((doCorr && doSmearMC) || doScaleData) corr->SmearMuon(&(event->mu_calo), event->eventinfo.EventNumber(), leptonType::MuonCalo);
	//if((doCorr && doSmearMC) || doScaleData) corr->SmearMuon(&(event->mu_staco), event->eventinfo.EventNumber(), leptonType::MuonStaco);
	//if(isDebugCall) printMuonInfo();

	//// getting the Eff values
	//if(doWeight && doScaleEfficiency) 
	//{
	//	muonStacoEff = corr->muonStacoEff;
	//	muonCaloEff = corr->muonCaloEff;
	//}
	//// Otherwise just fill them with 1
	//else
	//{
	//	for(Int_t i = 0; i < event->mu_staco.n(); i++)
	//		muonStacoEff.push_back(1);

	//	for(Int_t i = 0; i < event->mu_calo.n(); i++)
	//		muonCaloEff.push_back(1);
	//}
	//
	//muonObj->FillMuon(&(event->mu_staco), leptonType::MuonStaco, muonStacoEff, isMC);
	//muonObj->FillMuon(&(event->mu_calo), leptonType::MuonCalo, muonCaloEff, isMC);

	//Bool_t passCutMu = muonObj->MuonCut(cutMuPass);
	//if(!passCut) return passCut;

	//// To Perform cut on electrons
	//// Var to store the smearing
	//vector<Double_t> elSmearVal;
	//vector<Double_t> elEff;
	//vector<Double_t> elResolution;

	//// Clearing the vars
	//electronObj->clearVars();	
	//electronObj->SetHist(Hist);	
	//electronObjLoose->clearVars();	
	//electronObjLoose->SetHist(Hist);	
	//// Smearing
	//if(doCorr && doSmearD0Z0 && isMC) corr->SmearD0Z0(el_cur, event->eventinfo.EventNumber());
	//if((doCorr && doSmearMC) || doScaleData || doEPcombination) 
	//{
	//	
	//	// Don't want to correct Nom in this loop. This will create a double smearing
	//	if(doSysTree)
	//	{
	//		for(Int_t i = 0; i < doSys::Nom; i++)
	//		{
	//			elSmearVal = corr->SmearElectron(el_cur, event->eventinfo.EventNumber(), runNumber_sf, i);
	//			corr->ClearVars();
	//		}
	//	}
	//	corr->ClearVars();	
	//	elSmearVal.clear();		
	//	elSmearVal = corr->SmearElectron(el_cur, event->eventinfo.EventNumber(), runNumber_sf);
	//}
	//// In cases where no smearing will be done
	//if(!doCorr || !doSmearMC) 
	//{
	//	elSmearVal.clear();
	//	for(Int_t i = 0; i < el_cur->n(); i++)
	//		elSmearVal.push_back(1);
	//}
	//// getting the Eff values
	//if(doWeight && doScaleEfficiency) elEff = corr->electronEff;
	//// Otherwise just fill them with 1
	//else 
	//{
	//	elEff.clear();
	//	for(Int_t i = 0; i < el_cur->n(); i++)
	//		elEff.push_back(1);
	//}
	//elResolution = corr->electronEpErr;
	//if(elResolution.size() == 0)
	//{
	//	for(Int_t i = 0; i < el_cur->n(); i++)
	//		elResolution.push_back(-1);
	//}
	//// Getting the clusterPt for getting the lepton ID
	//vector<Double_t> el_bfEP_clPt;
	//el_bfEP_clPt.clear();
	//// Sanity check
 //	if( el_cur->n() != (Int_t) corr->bfEP_cl_pt.size() && doCorr) cout<<"Error: beforeEP_cl_pt has a different size than electron container"<<endl;
 //	for(Int_t i = 0; i < el_cur->n(); i++)
 //	{
 //		if(doCorr) el_bfEP_clPt.push_back(corr->bfEP_cl_pt[i]);
 //		else el_bfEP_clPt.push_back((*el_cur)[i].cl_pt());
 //	}

	//electronObj->FillElectron(el_cur, leptonType::ElectronGSF, elSmearVal, elEff,elResolution, el_bfEP_clPt ,isMC);
	//electronObjLoose->FillElectron(el_cur, leptonType::ElectronGSF, elSmearVal, elEff,elResolution, el_bfEP_clPt, isMC);

	//Bool_t passCutEl = electronObj->ElectronCut(cutElPass, getNVertex(2));
	//passCutEl = electronObjLoose->ElectronCut(cutElLoosePass, getNVertex(2), true);

	//// Photon Smearing
	//vector<Double_t> phSmearVal;
	//if((doCorr && doSmearMC) || doScaleData) phSmearVal = corr->SmearPhoton(&(event->ph), event->eventinfo.EventNumber(), runNumber_sf);
	//// In cases where no smearing will be done
	//if(!doCorr || !doSmearMC) 
	//{
	//	phSmearVal.clear();
	//	for(Int_t i = 0; i < el_cur->n(); i++)
	//		phSmearVal.push_back(1);
	//}

	//// To Perform cut on jets 
	//// Clearing the vars
	//jetsObj->clearVars();
	//jetsObjTruth->clearVars();
	//jetsObj_Fid->clearVars();
	//jetsObjTruth_Fid->clearVars();
	//// Calibrate jets
	//Double_t mu = -9999;
	//mu = event->eventinfo.averageIntPerXing();
	//if(isMC)
	//{
	//	if(dataYear == 2012)  mu = (event->eventinfo.lbn()==1&&int(mu+0.5)==1)?0.:mu;
	//}
	//Hist->muInteractionHist->Fill(mu);
	//Hist->muInteractionHistPileupW->Fill(mu,getPileupWeight());

	//Double_t rhoKt = -1;
	//if(dataYear == 2012) rhoKt = event->Eventshape.rhoKt4EM();
	//if(doCorr && doJetCal) corr->CalibrateJet(&(event->jet_akt4topoem), dataYear, rhoKt, mu, getNVertex(2));
	//jetsObj->FillJets(&(event->jet_akt4topoem), jetsType::AntiKt4TopoEM, isMC, event->eventinfo.EventNumber());
	//Bool_t passCutJets = jetsObj->JetsCut(cutJetsPass, event->eventinfo.RunNumber());
	//// True jets
	//jetsObjTruth->FillJetsTruth(&(event->jet_antikt4truth), jetsType::AntiKt4TopoEMTruth, isMC);
	//Bool_t passCutJetsTruth = jetsObjTruth->JetsCut(cutJetsPass, event->eventinfo.RunNumber());

	//// Jets for Fudical Cross-Section
	//jetsObj_Fid->FillJets(&(event->jet_akt4topoem), jetsType::AntiKt4TopoEM_Fid, isMC, event->eventinfo.EventNumber());
	//Bool_t passCutJets_Fid = jetsObj_Fid->JetsCut(cutJetsPass, event->eventinfo.RunNumber());
	//// True jets
	//jetsObjTruth_Fid->FillJetsTruth(&(event->jet_antikt4truth), jetsType::AntiKt4TopoEMTruth_Fid, isMC);
	//Bool_t passCutJetsTruth_Fid = jetsObjTruth_Fid->JetsCut(cutJetsPass, event->eventinfo.RunNumber());

	//// Reading the muon, electrons and jet events
	//clearVar();
	//jetsOverlap = jetsObj->getJetsVec();	
	//muOverlap = muonObj->getMuonVec();
	//elOverlap = electronObj->getElectronVec();
	//elLooseOverlap = electronObjLoose->getElectronVec();
	//jetsTruthEvent = jetsObjTruth->getJetsVec();
	//jetsOverlap_Fid = jetsObj_Fid->getJetsVec();
	//jetsTruthEvent_Fid = jetsObjTruth_Fid->getJetsVec();

	//// Removing the overlap
	//RemoveOverlap();

	//// Channel Specific Cuts
	//Bool_t pass4MuCut = false;
	//Bool_t pass4ElCut = false;
	//Bool_t pass2L2LCut = false;
	//Bool_t passlleeCut = false;
	//Bool_t passCutCH = false;
	//if(do4Mu && !passCutCH) pass4MuCut = CutFlow4Mu(eventWeight);
	//passCutCH = passCutCH | pass4MuCut;

	//if(do2L2L && !passCutCH) pass2L2LCut = CutFlow2L2L(eventWeight, quadType::El2Mu2);
	//passCutCH = passCutCH | pass2L2LCut;

	//if(do2L2L && !passCutCH) pass2L2LCut = CutFlow2L2L(eventWeight, quadType::Mu2El2);
	//passCutCH = passCutCH | pass2L2LCut;

	////if(do2L2L && !passCutCH) pass2L2LCut = CutFlow2L2L(eventWeight);
	////passCutCH = pass2L2LCut;

	//if(do4El && !passCutCH) pass4ElCut = CutFlow4El(eventWeight);
	//passCutCH = passCutCH | pass4ElCut;	

	//passCut = pass4MuCut | pass4ElCut | pass2L2LCut;
	//if(pass4MuCut) {cut4MuPass[cutFlowCH::Final]++; Hist->cut4MuPassHistW->Fill(cutFlowCH::Final);}
	//if(pass4ElCut) {cut4ElPass[cutFlowCH::Final]++; Hist->cut4ElPassHistW->Fill(cutFlowCH::Final);}
	//if(pass2L2LCut) {cut2L2LPass[cutFlowCH::Final]++; Hist->cut2L2LPassHistW->Fill(cutFlowCH::Final);}


	//// CR
	//if(dollee) passlleeCut = CutFlowllee(eventWeight);

	//// Printing the event list
	//if(printEventList) PrintEventList(pass4MuCut, pass4ElCut, pass2L2LCut);
	//
	//// Cleaning up for this event
	//muonStacoEff.clear();	
	//muonCaloEff.clear();
	//elSmearVal.clear();
	//elEff.clear();
	//elResolution.clear();
	//phSmearVal.clear();


	//return passCut;

}

void HiggsAnalysis::InitializeVar()
{
	cout << "Variables were initialized..." << endl;
}

void HiggsAnalysis::SetOutputFilePath(string newFilePath)
{
	m_outputFilePath = newFilePath;
	cout << "Output file set..." << endl;
}