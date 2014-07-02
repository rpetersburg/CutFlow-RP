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

HiggsAnalysis::HiggsAnalysis(TString tFileName_xAOD) : m_fileName_xAOD(tFileName_xAOD)
{
	xAOD::Init( "CutFlow4Lep_xAOD" );

	auto_ptr<TFile> ifile(TFile::Open(m_fileName_xAOD, "READ"));
	m_event_xAOD = new xAOD::TEvent(xAOD::TEvent::kClassAccess);
	m_event_xAOD->readFrom(ifile.get());
}

HiggsAnalysis::~HiggsAnalysis()
{
	delete m_physicsTree;
	delete m_event;	
}

void HiggsAnalysis::analyzeTree()
{
	Int_t entries = m_physicsTree->GetEntries();

	TChain* chain = dynamic_cast<TChain*> (m_physicsTree);
	m_currFileName = chain->GetFile()->GetPath();
	initCurrFileNameVec(); // Set m_currFileNameVec, a vector of the file name string

	for (Int_t entry = 0; entry < entries; entry++)
	{
		Long64_t currEntry = entry;
		if (chain)
		{
			currEntry = chain->LoadTree(currEntry);
		}
		analyzeTreeEvent(currEntry);
	}

	cout << "Tree was analyzed..." << endl;
}

void HiggsAnalysis::analyzeTree_xAOD()
{
	Int_t entries = m_event_xAOD->getEntries();

	m_currFileName = m_fileName_xAOD;
	initCurrFileNameVec(); // Set m_currFileNameVec, a vector of the file name string

	for (Long64_t entry = 0; entry < entries; entry++)
	{
		analyzeTreeEvent(entry);
	}
}

void HiggsAnalysis::analyzeTreeEvent(Long64_t eventNumber)
{
	// Initialize the D3PD event to get values for current event number
	if (eventNumber >= 0) 
	{
		m_event->GetEntry(eventNumber);
		m_event_xAOD->getEntry(eventNumber);
	}

	initIsMC(); // Checking if MC or Data

	if (m_isMC)
	{
		m_currDataCalibration = -1;
		initMCCollection();
	}
	else
	{
		m_currMCCollection = -1;
		initDataCalibration();
	}
	
	// Instantiate the Pileup Reweighting Tool
	Root::TPileupReweighting *pileupReweightingTool = (new PileupReweightTool(m_dataYear, m_currMCCollection, m_currDataCalibration))->getTool();

	initDataYear(); // Set m_dataYear for the event
	initIsTauSampleAndMCGenerator(); // Set m_isTauSample and m_mcGenerator
	initRunNumberSfAndLbnSf(pileupReweightingTool); // Set m_runNumber_sf and m_lbn_sf	
	initCurrElectron(); // Set m_currElectron
	initSampleType(); // Set m_sampleType
	if (m_isMC) initMCChannelNumber(); // Set m_mcChannelNumber
	if (m_isMC) initMCRunNumber(); // Set m_mcRunNumber

	// Getting the mass of MC Higgs using Calculation object
	Double_t higgsMass = 0;
	if (m_isMC) higgsMass = (new MCHiggsMass(m_event, m_currFileNameVec))->getMass();


	// Weights

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


	// Preselection Cuts

	// Ends event analysis if Data Preselection Cut is not passed
	if (!m_isMC)
	{
		DataPreselection *dataPreselectionObj = new DataPreselection(m_event);
		if (!dataPreselectionObj->passedCut()) return;
	}
	// Ends event analysis if Vertex Cut is not passed
	VertexCut *vertexCutObj = new VertexCut(m_event);
	if (!vertexCutObj->passedCut()) return;


	// Triggers

	// Ends event analysis if no leptons pass the trigger
	ElectronTrigger *electronTriggerTool = new ElectronTrigger(m_event, m_runNumber_sf);
	MuonTrigger *muonTriggerTool = new MuonTrigger(m_event, m_runNumber_sf);
	DiElectronTrigger *diElectronTriggerTool = new DiElectronTrigger(m_event, m_runNumber_sf);
	DiMuonTrigger *diMuonTriggerTool = new DiMuonTrigger(m_event, m_runNumber_sf);
	ElectronMuonTrigger *electronMuonTriggerTool = new ElectronMuonTrigger(m_event, m_runNumber_sf);

	if (!(electronTriggerTool->passedTrigger()   | muonTriggerTool->passedTrigger()   |
				diElectronTriggerTool->passedTrigger() | diMuonTriggerTool->passedTrigger() |
				electronMuonTriggerTool->passedTrigger()))
		return;


	// Smearing

	// D0Z0 Smearing (both electrons and muons)
	if (m_dataYear == 2012 && m_isMC) (new D0Z0Smear(m_event))->executeSmear();

	// Muon Smearing
	MuonCaloSmear *muonCaloSmearObj = new MuonCaloSmear(m_event, pileupReweightingTool);
	MuonStacoSmear *muonStacoSmearObj = new MuonStacoSmear(m_event, pileupReweightingTool);
	muonStacoSmearObj->executeSmear();
	muonCaloSmearObj->executeSmear();

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

	
	// Setting the Particle Objects

	vector<Muon*> muonStacoVec, muonCaloVec;
	vector<Electron*> electronVec, electronLooseVec;
	vector<Jets*> jetsVec, jetsTruthVec, jetsVec_Fid, jetsTruthVec_Fid;
	
	for (Int_t i = 0; i < m_event->mu_staco.n(); i++)
		muonStacoVec.push_back(new Muon(m_event, &(m_event->mu_staco[i])));

	for (Int_t i = 0; i < m_event->mu_calo.n(); i++)
		muonCaloVec.push_back(new Muon(m_event, &(m_event->mu_calo[i])));

	for (Int_t i = 0; i < m_currElectron->n(); i++)
	{
		electronVec.push_back(new Electron(m_event, &(*m_currElectron)[i]));
		electronLooseVec.push_back(new Electron(m_event, &(*m_currElectron)[i]));
	}

	for (Int_t i = 0; i < m_event->jet_akt4topoem.n(); i++)
	{
		jetsVec.push_back(new Jets(m_event, &(m_event->jet_akt4topoem[i]), JetsType::AntiKt4TopoEM));
		jetsVec_Fid.push_back(new Jets(m_event, &(m_event->jet_akt4topoem[i]), JetsType::AntiKt4TopoEM_Fid));
	}

	for (Int_t i = 0; i < m_event->jet_antikt4truth.n(); i++)
	{
		jetsTruthVec.push_back(new Jets(m_event, &(m_event->jet_antikt4truth[i]), JetsType::AntiKt4TopoEMTruth));
		jetsTruthVec_Fid.push_back(new Jets(m_event, &(m_event->jet_antikt4truth[i]), JetsType::AntiKt4TopoEMTruth_Fid));
	}

	// Setting the smear and efficiency values from smear object to muon vectors
	for (vector<Muon*>::size_type i = 0; i != muonStacoVec.size(); i++)
	{
		muonStacoVec[i]->setSmear(muonStacoSmearObj->getSmear()[i]);
		if (m_isMC)	muonStacoVec[i]->setEff(muonStacoSmearObj->getEff()[i]);
		else muonStacoVec[i]->setEff(1); 
	}
	for (vector<Muon*>::size_type i = 0; i != muonCaloVec.size(); i++)
	{
		muonCaloVec[i]->setSmear(muonCaloSmearObj->getSmear()[i]);
		if (m_isMC) muonCaloVec[i]->setEff(muonCaloSmearObj->getEff()[i]);
		else muonCaloVec[i]->setEff(1);
	}
	// Setting the smear, effiency, resolution, and cluster pT for electron vector
	for (vector<Electron*>::size_type i = 0; i != electronVec.size(); i++)
	{
		if (m_isMC) electronVec[i]->setEff(electronSmearObj->getEff()[i]);
		else electronVec[i]->setEff(1);
		electronVec[i]->setSmear(electronSmearObj->getSmear()[i]);
		electronVec[i]->setResolution(electronSmearObj->getMomentumError()[i]);
		electronVec[i]->setClusterpT(electronSmearObj->getbfEP_cl_pT()[i]);
	}


	// Particle Specific Cuts

	// Muon Cut
	MuonCut *muonStacoCutTool = new MuonCut(m_event, &muonStacoVec);
	MuonCut *muonCaloCutTool = new MuonCut(m_event, &muonCaloVec);
	// passedCut() functions execute cuts and return true if valid
	muonStacoCutTool->executeCut();
	muonCaloCutTool->executeCut();
	muonStacoVec = muonStacoCutTool->getCutParticleVec();
	muonCaloVec = muonCaloCutTool->getCutParticleVec();

	// Electron Cut
	ElectronCut *electronCutTool = new ElectronCut(m_event, &electronVec);
	electronCutTool->executeCut();
	electronVec = electronCutTool->getCutParticleVec();

	electronCutTool->setInitParticleVec(&electronLooseVec);
	electronCutTool->setIsLoose(true);
	electronCutTool->executeCut();
	electronLooseVec = electronCutTool->getCutParticleVec();

	// Jets Cut
	JetsCut *jetsCutTool = new JetsCut(m_event, &jetsVec);

	jetsCutTool->setInitParticleVec(&jetsVec);
	jetsCutTool->executeCut();
	jetsVec = jetsCutTool->getCutParticleVec();

	jetsCutTool->setInitParticleVec(&jetsTruthVec);
	jetsCutTool->executeCut();
	jetsTruthVec = jetsCutTool->getCutParticleVec();

	jetsCutTool->setInitParticleVec(&jetsVec_Fid);
	jetsCutTool->executeCut();
	jetsVec_Fid = jetsCutTool->getCutParticleVec();

	jetsCutTool->setInitParticleVec(&jetsTruthVec_Fid);
	jetsCutTool->executeCut();
	jetsTruthVec_Fid = jetsCutTool->getCutParticleVec();


	// Particle Specific Overlap Removal

	// Muon Overlap
	MuonOverlap *muonOverlapTool = new MuonOverlap(&muonStacoVec);
	muonOverlapTool->removeOverlap();
	muonStacoVec = muonOverlapTool->getGoodParticleVec();

	muonOverlapTool->setInitParticleVec(&muonCaloVec);
	muonOverlapTool->removeOverlap();
	muonCaloVec = muonOverlapTool->getGoodParticleVec();

	// Electron Overlap
	ElectronOverlap *electronOverlapTool = new ElectronOverlap(&electronVec);
	electronOverlapTool->removeOverlap();
	electronVec = electronOverlapTool->getGoodParticleVec();

	electronOverlapTool->setInitParticleVec(&electronLooseVec);
	electronOverlapTool->removeOverlap();
	electronLooseVec = electronOverlapTool->getGoodParticleVec();

	
	// Multiple Particle Overlap Removal (first listed is primary particle)

	// Electron Muon Overlap
	ElectronMuonOverlap *electronMuonOverlapTool = new ElectronMuonOverlap(&electronVec, &muonCaloVec, &muonStacoVec);
	electronMuonOverlapTool->removeOverlap();
	electronVec = electronMuonOverlapTool->getGoodParticleVec();

	electronMuonOverlapTool->setInitParticleVec(&electronLooseVec);
	electronMuonOverlapTool->removeOverlap();
	electronLooseVec = electronMuonOverlapTool->getGoodParticleVec();

	// Muon Electron Overlap (only have to do for Calo Muons)
	MuonElectronOverlap *muonElectronOverlapTool = new MuonElectronOverlap(&muonCaloVec, &electronVec);
	muonElectronOverlapTool->removeOverlap();
	muonCaloVec = muonElectronOverlapTool->getGoodParticleVec();

	// Jets Electron Overlap
	JetsElectronOverlap *jetsElectronOverlapTool = new JetsElectronOverlap(&jetsVec, &electronVec);
	jetsElectronOverlapTool->removeOverlap();
	jetsVec = jetsElectronOverlapTool->getGoodParticleVec();

	jetsElectronOverlapTool->setInitParticleVec(&jetsVec_Fid);
	jetsElectronOverlapTool->removeOverlap();
	jetsVec_Fid = jetsElectronOverlapTool->getGoodParticleVec();


	// Combine Muon Vectors
	vector<Muon*> muonVec;
	for (vector<Muon*>::size_type i = 0; i != muonStacoVec.size(); i++)
		muonVec.push_back(muonStacoVec[i]);
	for (vector<Muon*>::size_type i = 0; i != muonCaloVec.size(); i++)
		muonVec.push_back(muonCaloVec[i]);


	// Quadlepton Cutflows
	CutFlow4mu *cutFlow4mu = new CutFlow4mu(m_event, m_currMCCollection, m_runNumber_sf, &muonVec);
	CutFlow4e *cutFlow4e = new CutFlow4e(&electronVec);

	if (muonTriggerTool->passedTrigger() | diMuonTriggerTool->passedTrigger())
	{
		
	}




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

// For MC simulations, sets the type of higgs sample
void HiggsAnalysis::initSampleType()
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
void HiggsAnalysis::initMCRunNumber()
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
void HiggsAnalysis::initCurrFileNameVec()
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

// Needs good documentation and some self-alterations.... Not sure exactly what is going on
void HiggsAnalysis::initMCCollection()
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

			if (find(mc11d, mc11d+nMC11d, prodTagNum) != mc11c + nMC11c)
				m_currMCCollection = MCCollection::MC11c;
			else if (find(mc11d, mc11d+nMC11d, prodTagNum) != mc11d + nMC11d)
				m_currMCCollection = MCCollection::MC11d;
			else if (find(mc12ab, mc12ab + nMC12ab, prodTagNum) != mc12ab + nMC12ab)
			{
				if (runNumber == 195847) m_currMCCollection = MCCollection::MC12a;
				else if (runNumber == 195848) m_currMCCollection = MCCollection::MC12b;
				else cout << "Error: HiggsAnalysis::setMCCollection(): Mc12ab run number not recognized" << endl;
			}
			else if (find(mc12c, mc12c + nMC12c, prodTagNum) != mc12c + nMC12c) m_currMCCollection = MCCollection::MC12c;
			else cout << "Error: HiggsAnalysis::setMCCollection(): No currMCCollection set" << endl;
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
	
void HiggsAnalysis::initIsTauSampleAndMCGenerator()
{
	// Checking if tau sample
	if (m_currFileName.Contains("noTau")) m_isTauSample = false;
	else m_isTauSample = true;

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
		m_isTauSample = false;
		m_mcGenerator = MCGeneratorName::Pythia;
	} 
}

void HiggsAnalysis::initRunNumberSfAndLbnSf(Root::TPileupReweighting *pileupReweightingTool)
{
	// Setting the run number (sf) and luminosity block number
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
}

void HiggsAnalysis::initCurrElectron()
{
	// 2012 defaults to GSF electrons; 2011 does not
	if (m_dataYear == 2011) m_currElectron = &(m_event->el_GSF);
	else if (m_dataYear == 2012) m_currElectron = &(m_event->el);
}