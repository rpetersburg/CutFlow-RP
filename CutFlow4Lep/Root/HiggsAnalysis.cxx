#include "CutFlow4Lep/HiggsAnalysis.h"

HiggsAnalysis::HiggsAnalysis()
{
	cout << "You reached the empty constructor" << endl;
}

HiggsAnalysis::HiggsAnalysis(TChain *tPhysicsTree) : m_physicsTree(tPhysicsTree)
{
	cout << "You reached the helpful constructor" << endl;
	m_event = new D3PDReader::Event();
	m_event->ReadFrom(m_physicsTree);

	m_countingHist = new TH1F("CountingHist", "CountingHist", 10, 0, 10);
}

HiggsAnalysis::~HiggsAnalysis()
{
	delete m_physicsTree;
	delete m_event;	
}

void HiggsAnalysis::analyzeTree()
{
	Int_t totNumEvents = m_physicsTree->GetEntries();

	for (Int_t iEvent = 0; iEvent < totNumEvents; iEvent++)
	{
		Long64_t currEvent = iEvent;
		TChain* chain = dynamic_cast<TChain*> (m_physicsTree);
		if (chain)
		{
			currEvent = chain->LoadTree(currEvent);
		}
		m_currFileName = chain->GetFile()->GetPath();
		analyzeTreeEvent(currEvent);
	}

	cout << "Tree was analyzed..." << endl;
}

void HiggsAnalysis::analyzeTreeEvent(Long64_t eventNumber)
{
	// Initialize the D3PD event to get values for current event number
	if(eventNumber >= 0) m_event->GetEntry(eventNumber);

	// Checking if MC or Data
	if(m_event->eventinfo.isSimulation()) m_isMC = true;
	else m_isMC = false;

	// Set m_dataYear, m_cmEnergy, and m_is2012 for the event
	setEventYear();

	// Getting the mass of MC Higgs using Calculation object
	Double_t higgsMass = 0;
	if (m_isMC) higgsMass = (new MCHiggsMass(m_event, m_currFileNameVec))->getMass();

	// Setting the sample type (m_sampleType)
	setSampleType();

	// Instantiate the Pileup Reweighting Tool
	Root::TPileupReweighting *pileupReweightingTool = (new PileupReweightTool(m_dataYear, m_currMCCollection, m_currDataCalibration))->getTool();

	// Getting the event's weight
	EventWeight *eventWeightObj = new EventWeight(m_event, higgsMass, m_sampleType, pileupReweightingTool);
	Double_t eventWeight = eventWeightObj->getWeight();
	Double_t eventJHUWeight = eventWeightObj->getJHUWeight();
	Double_t eventggFWeight = eventWeightObj->getggFWeight();

	// Filling the counting histogram with relevant weights
	m_countingHist->Fill(1);
	m_countingHist->Fill(2, eventWeight);
	m_countingHist->Fill(3, eventWeight);
	m_countingHist->Fill(4, eventWeight/eventggFWeight/eventJHUWeight);
	m_countingHist->Fill(5, eventWeight/eventJHUWeight);

	// Get MC Channel Number
	if (m_isMC) m_mcChannelNumber = m_event->eventinfo.mc_channel_number();

	setTauSampleAndGenerator();
	
	// 2012 defaults to GSF electrons; 2011 does not
	if (m_dataYear == 2011) m_currElectron = &(m_event->el_GSF);
	else if (m_dataYear == 2012) m_currElectron = &(m_event->el);

	if (m_isMC)
	{
		m_currDataCalibration = -1;
		setMCCollection();
	}
	else
	{
		m_currMCCollection = -1;
		setDataCalibration();
	}

	// Ends event analysis if Data Preselection Cut is not passed
	if (!m_isMC)
	{
		DataPreselection *dataPreselectionObj = new DataPreselection(m_event);
		if (!dataPreselectionObj->passedCut()) return;
	}
	// Ends event analysis if Vertex Cut is not passed
	VertexCut *vertexCutObj = new VertexCut(m_event);
	if (!vertexCutObj->passedCut) return;

	// Setting the run number and luminosity block number
	if (m_isMC)
	{
		// Set random run number and luminosity to mimic randomness of data
		pileupReweightingTool->SetRandomSeed(314159 + m_event->eventinfo.mc_channel_number() * 2718 + m_event->eventinfo.EventNumber());
		m_runNumber_sf = pileupReweightingTool->GetRandomRunNumber(m_event->eventinfo.RunNumber());
		m_lbn_sf = pileupReweightingTool->GetRandomLumiBlockNumber(m_runNumber_sf);
	}
	else
	{
		m_runNumber_sf = m_event->eventinfo.RunNumber();
		m_lbn_sf = m_event->eventinfo.lbn();
	}

	// Ends event analysis if no leptons pass the trigger
	if (!((new ElectronTrigger(m_event, m_dataPeriod, m_runNumber_sf))->passedTrigger() |
				(new MuonTrigger(m_event, m_dataPeriod, m_runNumber_sf))->passedTrigger() |
				(new DiElectronTrigger(m_event, m_dataPeriod, m_runNumber_sf))->passedTrigger() |
				(new DiMuonTrigger(m_event, m_dataPeriod, m_runNumber_sf))->passedTrigger() |
				(new ElectronMuonTrigger(m_event, m_dataPeriod, m_runNumber_sf))->passedTrigger()))
		return;

	// Smearing

	// D0Z0 Smearing (both electrons and muons)
	if (m_dataYear == 2012 && m_isMC) (new D0Z0Smear(m_event))->executeSmear();

	// Muon Smearing
	MuonCaloSmear *muonCaloSmearObj = new MuonCaloSmear(m_event, pileupReweightingTool);
	MuonStacoSmear *muonStacoSmearObj = new MuonStacoSmear(m_event, pileupReweightingTool);
	muonStacoSmearObj->executeSmear();
	muonCaloSmearObj->executeSmear();

	vector<Double_t> muonStacoSmear = muonStacoSmearObj->getSmear();
	vector<Double_t> muonCaloSmear = muonCaloSmearObj->getSmear();

	// Setting the muon efficiency values when MC
	vector<Double_t> muonStacoEff, muonCaloEff;
	if (m_isMC)
	{
		muonStacoEff = muonStacoSmearObj->getEff();
		muonCaloEff = muonCaloSmearObj->getEff();
	}
	// Otherwise the efficiencies are 1
	else
	{
		for (Int_t i = 0; i < m_event->mu_staco.n(); i++)
			muonStacoEff.push_back(1);
		for (Int_t i = 0; i < m_event->mu_calo.n(); i++)
			muonCaloEff.push_back(1);
	}

	// Electron Smearing
	ElectronSmear *electronSmearObj = new ElectronSmear(m_event, m_currMCCollection, m_currDataCalibration, m_runNumber_sf);
	electronSmearObj->executeSmear();

	vector<Double_t> electronSmear = electronSmearObj->getSmear();

	vector<Double_t> electronEff;
	if (m_isMC) electronEff = electronSmearObj->getEff();
	else
	{
		for (Int_t i = 0; i < m_currElectron->n(); i++)
			electronEff.push_back(1);
	}
	vector<Double_t> electronResolution = electronSmearObj->getMomentumError();
	if (electronResolution.size() == 0)
	{
		for (Int_t i = 0; i < m_currElectron->n(); i++)
			electronResolution.push_back(-1);
	}
	vector<Double_t> electron_bfEP_clpT;
	for (Int_t i = 0; i < m_currElectron->n(); i++)
		electron_bfEP_clpT.push_back(electronSmearObj->getbfEP_cl_ET()[i]);

	// Photon Smearing
	PhotonSmear *photonSmearObj = new PhotonSmear(m_event, m_currMCCollection, m_currDataCalibration, m_runNumber_sf);
	photonSmearObj->executeSmear();

	vector<Double_t> photonSmear = photonSmearObj->getSmear();

	// Jet Calibration
	JetCalibration *jetCalibrationObj = new JetCalibration(m_event, m_currMCCollection);
	jetCalibrationObj->executeCorrection();


	//cutPass[cutFlow::DataPreselection]++;
	//cutMuPass[cutMuFlow::DataPreselection] += (event->mu_staco.n() + event->mu_calo.n());
	//cutElPass[cutElFlow::DataPreselection] += (el_cur->n());
	//cutJetsPass[cutJetsFlow::DataPreselection] += event->jet_akt4topoem.n();
	//Hist->cutPassHistW->Fill(cutFlow::DataPreselection, eventWeight);	
	//
	//cutPass[cutFlow::Preselection]++;
	//cutMuPass[cutMuFlow::Preselection] += (event->mu_staco.n() + event->mu_calo.n());
	//cutElPass[cutElFlow::Preselection] += (el_cur->n());
	//cutJetsPass[cutJetsFlow::Preselection] += event->jet_akt4topoem.n();
	//Hist->cutPassHistW->Fill(cutFlow::Preselection, eventWeight);

	//if(passCut4Mu) {cutPass[cutFlow::Trigger4Mu] ++; Hist->cutPassHistW->Fill(cutFlow::Trigger4Mu, eventWeight);}
	//if(passCut4e) {cutPass[cutFlow::Trigger4e] ++;; Hist->cutPassHistW->Fill(cutFlow::Trigger4e, eventWeight);}
	//
	//// For Trig eff studies
	//if(anaType == doAnalysis::trigeff4l) passCut = true;
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

void HiggsAnalysis::setOutputFilePath(string newFilePath)
{
	m_outputFilePath = newFilePath;
	cout << "Output file set..." << endl;
}

void HiggsAnalysis::setEventYear()
{
	// Setting the event year and CM Energy of the data
	Long64_t runNumber = m_event->eventinfo.RunNumber();
	if ( (runNumber >= 177531) && (runNumber <= 191933) )
	{
		m_dataYear = 2011;
		m_cmEnergy = 7.0;
		m_is2012 = false;
	}
	else if ( runNumber > 191933 )
	{
		m_dataYear = 2012;
		m_cmEnergy = 8.0;
		m_is2012 = true;
	}
	else cout<<"Error: HiggsAnalysis::setOutputFilePath(): Event run number not recognized: "<< runNumber <<endl;
}

// For MC simulations, sets the type of higgs sample
void HiggsAnalysis::setSampleType()
{
	for (vector<Int_t>::size_type i = 0; i != m_currFileNameVec.size(); i++)
	{
		// Using only the first part of file name
		if (m_currFileNameVec[i].Contains("mc11_7TeV") || m_currFileNameVec[i].Contains("mc12_8TeV"))
		{
			// Finding the sample type
			Int_t sampleIndex = i + 2;
			TString sampleName = m_currFileNameVec[sampleIndex];

			if (sampleName.Contains("_ZZp") ||
				sampleName.Contains("_ZpZp") )			m_sampleType = SampleType::ggF_ZpZp;
			else if (sampleName.Contains("ggH"))	m_sampleType = SampleType::ggF;
			else if (sampleName.Contains("VBF"))	m_sampleType = SampleType::VBF;
			else if (sampleName.Contains("WH"))		m_sampleType = SampleType::WH;
			else if (sampleName.Contains("ZH"))		m_sampleType = SampleType::ZH;
			else if (sampleName.Contains("ttH"))	m_sampleType = SampleType::ttH;
			else if (sampleName.Contains("qqH"))	m_sampleType = SampleType::qqF;
			else																	m_sampleType = SampleType::Background;
		}
	}

}

// Finds the MC Run Number within the file name
void HiggsAnalysis::setMCRunNumber()
{
	for (vector<Int_t>::size_type i = 0; i != m_currFileNameVec.size(); i++)
	{
		// Finding the string before the run number
		if (m_currFileNameVec[i].Contains("mc11_7TeV") || m_currFileNameVec[i].Contains("mc12_8TeV"))
		{
			// Finding the run number
			Int_t runIndex = i + 1;
			TString runString = m_currFileNameVec[runIndex];
			m_mcRunNumber = runString.Atoi();
			return;
		}
	}
	cout << "Error: HiggsAnalysis::setMCRunNumber(): MC Run Number not found" << endl;
	return;
}

// Splits the file name by '.' and adds parts to a vector
void HiggsAnalysis::setCurrFileNameVec()
{
	// Splitting the file path
	TObjArray *splitFileName = m_currFileName.Tokenize(".");
	if (splitFileName->GetEntriesFast())
	{
		TIter iString(splitFileName);
		TObjString* objString = 0;
		while ( (objString = (TObjString*)iString()) )
			m_currFileNameVec.push_back( objString->GetString() );
	}
}

void HiggsAnalysis::setDataCalibration()
{
	if (m_dataYear == 2011) m_currDataCalibration = DataCalibType::y2011d;
	else if (m_dataYear == 2012) m_currDataCalibration = DataCalibType::y2012c;
}

// Needs good documentation and some self-alterations.... Not sure exactly what is going on
void HiggsAnalysis::setMCCollection()
{
	Int_t mc11c[] 	= {1272,1273,1274,1299,1300,1309,1310,1349,1350,1351,1352,1353,1370,1372,1378,1571};
	Int_t mc11d[] 	= {1786};
	Int_t mc12ab[] 	= {1468,1469,1470,1472,1479,1482,1484,1485,1486,1499,1504,1581,1586,1589,1599,1609,1610,1611,1716,1773,1773,1776};	
	Int_t mc12c[] 	= {1737,1741,1746,1748,1771,1798,1799,1831,1832};

	TString name = m_currFileName;

	TObjArray *parts = name.Tokenize(".");
	vector<TString> partName;
	if (parts->GetEntriesFast())
	{
		TIter iString(parts);
		TObjString *objString = 0;
		while ( (objString=(TObjString*)iString()) ) partName.push_back(objString->GetString());
	}

	Long64_t runNumber = m_event->eventinfo.RunNumber();

	for (Int_t i = 0; i < (Int_t) partName.size(); i++)
	{
		if (partName[i].Contains("_s"))
		{
			TObjArray *parts = partName[i].Tokenize("_");
			vector<TString> prodTag;
			if (parts->GetEntriesFast())
			{
				TIter iString(parts);
				TObjString* os = 0;
				while ((os=(TObjString*)iString())) prodTag.push_back(os->GetString());
			}
			
			TString sProdTag = prodTag[1];
			sProdTag = sProdTag(1,4);
			Int_t prodTagNum = sProdTag.Atoi();

			Int_t nMC11c  =	(sizeof(mc11c)/sizeof(*mc11c));
			Int_t nMC11d  =	(sizeof(mc11d)/sizeof(*mc11d));
			Int_t nMC12ab =	(sizeof(mc12ab)/sizeof(*mc12ab));
			Int_t nMC12c  =	(sizeof(mc12c)/sizeof(*mc12c));

			if (find(mc11d, mc11d+nMC11d, prodTagNum) != mc11c + nMC11c) {m_currMCCollection = MCCollection::MC11c;}
			else if (find(mc11d, mc11d+nMC11d, prodTagNum) != mc11d + nMC11d) {m_currMCCollection = MCCollection::MC11d;}
			else if (find(mc12ab, mc12ab + nMC12ab, prodTagNum) != mc12ab + nMC12ab)
			{
				if (runNumber == 195847) m_currMCCollection = MCCollection::MC12a;
				else if (runNumber == 195848) m_currMCCollection = MCCollection::MC12b;
				else cout << "Error: AnalyzeTreeEvent: Mc12ab run number not recognized" << endl;
			}
			else if (find(mc12c, mc12c + nMC12c, prodTagNum) != mc12c + nMC12c) {m_currMCCollection = MCCollection::MC12c;}
			else {cout << "Error: HiggsAnalysis::setMCCollection(): " << endl;}
		}
	}

	// Specific issue if the sample production type is ggF_ZpZp
	if (m_sampleType == SampleType::ggF_ZpZp)
	{
		if (m_dataYear == 2011) m_currMCCollection = MCCollection::MC11c;
		else if (m_dataYear == 2012)
		{
			if (m_event->eventinfo.RunNumber() == 195847) m_currMCCollection = MCCollection::MC12a;
			else if (m_event->eventinfo.RunNumber() == 195848) m_currMCCollection = MCCollection::MC12b;
			else cout << "Error: HiggsAnalysis::AnalyzeTreeEvent: MC12ab run number not recognized" << endl;
		}
	}
}

void HiggsAnalysis::setDataPeriod()
{
	Int_t runNumber = m_event->eventinfo.RunNumber();

	if (runNumber >= 177986 && runNumber <= 180481) 
		m_dataPeriod = DataPeriod::run2011_BD;
	else if (runNumber >= 180614 && runNumber <= 184169)
		m_dataPeriod = DataPeriod::run2011_EH;
	else if (runNumber >= 185353 && runNumber <= 187815)
		m_dataPeriod = DataPeriod::run2011_IK;
	else if (runNumber >= 188902 && runNumber <= 191933)
		m_dataPeriod = DataPeriod::run2011_LM;
	else if (runNumber >= 195847 && runNumber <= 999999)
		m_dataPeriod = DataPeriod::run2012_All;
}
	
void HiggsAnalysis::setTauSampleAndGenerator()
{
	// Checking if tau sample
	if (m_currFileName.Contains("noTau")) m_tauSample = false;
	else m_tauSample = true;

	// Check mc generator name
	if (m_currFileName.Contains("Pythia")) m_mcGenerator = MCGeneratorName::Pythia;

	// Set tau sample boolean and mc generator based on channel number
	if((m_mcChannelNumber>=169716 && m_mcChannelNumber<=169717) ||  // 2011 JHU
     (m_mcChannelNumber>=167604 && m_mcChannelNumber<=167605) ||  // 2011 JHU
     (m_mcChannelNumber>=167607 && m_mcChannelNumber<=167607) ||  // 2012 JHU
     (m_mcChannelNumber>=169710 && m_mcChannelNumber<=169711) ||  // 2012 JHU
     (m_mcChannelNumber>=167124 && m_mcChannelNumber<=167125) ||  // 2012 JHU
     (m_mcChannelNumber>=167127 && m_mcChannelNumber<=167127) ||
		 (m_mcChannelNumber>=167600 && m_mcChannelNumber<=167603) ||  // 2011 JHU
     (m_mcChannelNumber>=167606 && m_mcChannelNumber<=167606) ||  // 2011 JHU
     (m_mcChannelNumber>=167120 && m_mcChannelNumber<=167123) ||  // 2012 JHU
     (m_mcChannelNumber>=167126 && m_mcChannelNumber<=167126) || 
	   (m_mcChannelNumber>=181990 && m_mcChannelNumber<=181996)     // mc12c JHU
	  ) 
	{  		  
		m_tauSample = false;
		m_mcGenerator = MCGeneratorName::Pythia;
	} 
}