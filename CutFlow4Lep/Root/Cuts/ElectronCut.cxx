#include "CutFlow4Lep/Cuts/ElectronCut.h"

ElectronCut::ElectronCut(D3PDReader::Event *tEvent, vector<Electron*> *tInitElectronVec) 
	: ParticleCuts(tEvent, tInitElectronVec)
{
	init();
}

ElectronCut::~ElectronCut()
{

}

void ElectronCut::executeCut()
{
	vector<Electron*>::iterator currElectronObj = m_initParticleVec->begin();
	for ( ; currElectronObj != m_initParticleVec->end(); ++currElectronObj)
	{
		if (passedElectronCut(*currElectronObj))
			m_cutParticleVec.push_back(*currElectronObj);
	}
}

Bool_t ElectronCut::passedCut()
{
	executeCut();

	if (m_cutParticleVec.size() > 0) return true;
	return false;
}

Bool_t ElectronCut::passedElectronCut(Electron *currElectronObj)
{
	D3PDReader::ElectronD3PDObjectElement *currElectron = currElectronObj->getElectron();
	// Author Cut
	if (currElectron->author() != 1 && currElectron->author() != 3)
		return false;
	if (!passedAuthorCut(currElectronObj)) return false;

	// Kinematics
	if (fabs(currElectron->cl_eta()) >= 2.47) return false;
	if (currElectron->cl_E() / cosh(currElectron->tracketa()) <= 7000) return false;

	// Object Quality
	if ((currElectron->OQ() & 1446) != 0) return false;

	// Z0 Cut
	if (fabs(currElectron->trackz0pvunbiased()) >= m_z0Cut) return false;

	return true; // If all cuts are passed
}

Bool_t ElectronCut::passedAuthorCut(Electron *currElectronObj)
{
	D3PDReader::ElectronD3PDObjectElement *currElectron = currElectronObj->getElectron();
	Bool_t PassElectronID = true;
	Int_t  author = currElectron->author();

	if(m_dataYear == 2011)
	{
		// Necessary variables to execute the cut
		Double_t Eta_cl_s2 = currElectron->etas2();
		Double_t Et_cl_s2= currElectron->cl_E() / cosh(Eta_cl_s2) / currElectronObj->getSmear();
		Double_t e237 = currElectron->E237();
		Double_t e277 = currElectron->E277();
		Double_t Had1 = currElectron->Ethad1();
		Double_t Had = currElectron->Ethad();		
		Double_t w1 = currElectron->ws3();
		Double_t w2 = currElectron->weta2();
		Double_t f1 = currElectron->f1(); 
		Double_t emax2 = currElectron->Emax2();
		Double_t emax = currElectron->emaxs1();
		Double_t emin = currElectron->Emins1();
		Double_t wstot = currElectron->wstot(); 
		Double_t fracm = currElectron->fside();
		Double_t f3 = currElectron->f3();

		Int_t		 nBlayer = currElectron->nBLHits();
		Int_t    nBlayerOutliers = currElectron->nBLayerOutliers(); 
		Int_t		 nPix = currElectron->nPixHits();
		Int_t		 nPixOutliers = currElectron->nPixelOutliers();
		Int_t		 nSCT = currElectron->nSCTHits(); 
		Int_t		 nSCTOutliers = currElectron->nSCTOutliers();
		Int_t		 nTRThigh =  currElectron->nTRTHighTHits();
		Int_t		 nTRThighOutliers =  currElectron->nTRTHighTOutliers();
		Int_t		 nTRT = currElectron->nTRTHits(); 
		Int_t		 nTRTOutliers = currElectron->nTRTOutliers();		
		Int_t		 nTRTXenonHits = 1.0; // currElectron->nTRTXenonHits();

		Double_t d0 = currElectron->trackd0_physics();
		Double_t deltaEta = currElectron->deltaeta1();
		Double_t deltaPhi = currElectron->deltaphi2(); 
		Double_t ep = 1.;

		Bool_t   expectBlayer = currElectron->expectHitInBLayer();
		EMAmbiguityType::AmbiguityResult amb = EMAmbiguityType::ELECTRON;

		// Int_t nSiOutliers = currElectron->nSCTOutliers() + currElectron->nPixelOutliers();

		Root::TAccept tAccept = m_electronTool2011->accept(Eta_cl_s2,
																											 fabs(Et_cl_s2),
																											 e237,    
																											 e277,    
																											 Had1, 
																											 Had,
																											 w1,      
																											 w2,      
																											 f1, 
																											 emax2,    
																											 emax,    
																											 emin, 
																											 wstot,
																											 fracm,   
																											 f3, 
																											 nBlayer,
																											 nBlayerOutliers,
																											 nPix, 
																											 nPixOutliers,
																											 nSCT, 
																											 nSCTOutliers,
																											 nTRThigh,         
																											 nTRThighOutliers,
																											 nTRT, 
																											 nTRTOutliers, 
																											 nTRTXenonHits,    
																											 d0,
																											 fabs(deltaEta), 
																											 deltaPhi, 
																											 ep,               
																											 expectBlayer,
																											 amb);
		PassElectronID = (Bool_t)tAccept;
	}
	else if (m_dataYear == 2012 && !useLikelihood) // or doMoriond
	{
		// Variables needed to set necessary variables
		Int_t		 nTRT = currElectron->nTRTHits(); 
		Int_t		 nTRTOutliers = currElectron->nTRTOutliers();
		Int_t		 nTRThigh = currElectron->nTRTHighTHits();
		Int_t		 nTRThighOutliers = currElectron->nTRTHighTOutliers();
		// Necessary variables to execute the cut
		Double_t Eta_cl_s2 = currElectron->etas2();
		Double_t Et_cl_s2= currElectron->cl_E() / cosh(Eta_cl_s2) / currElectronObj->getSmear();
		Double_t rHad = currElectron->Ethad()/Et_cl_s2;
		Double_t rHad1 = currElectron->Ethad1()/Et_cl_s2;
		Double_t Reta = currElectron->reta(); 
		Double_t w2 = currElectron->weta2();
		Double_t f1 = currElectron->f1();
		Double_t f3 = currElectron->f3();
		Double_t wstot = currElectron->wstot();
		Double_t DEmaxs1 = fabs(currElectron->emaxs1() + currElectron->Emax2()) > 0.
										 ? (currElectron->emaxs1() - currElectron->Emax2()) / (currElectron->emaxs1() + currElectron->Emax2()) 
										 : 0.;
		Double_t deltaEta = currElectron->deltaeta1();

		Int_t    nSi = currElectron->nSiHits();
		Int_t		 nSiDeadSensors = currElectron->nSCTDeadSensors() + currElectron->nPixelDeadSensors();
		Int_t    nPix = currElectron->nPixHits();
		Int_t		 nPixDeadSensors = currElectron->nPixelDeadSensors();

		Double_t deltaPhiRescaled = currElectron->deltaphiRescaled();
		Double_t dpOverp = 0;
		for (Int_t i = 0; i < (Int_t) currElectron->refittedTrack_LMqoverp().size(); ++i)
		{
			if( (currElectron->refittedTrack_author().at(i) ==4))
				dpOverp = 1 - (currElectron->trackqoverp() / currElectron->refittedTrack_LMqoverp().at(i));
		}
		Double_t rTRT = (nTRT + nTRTOutliers) > 0 
									? (double) ((nTRThigh + nTRThighOutliers) / (nTRT + nTRTOutliers))
									: 0.;
		Int_t		 nTRTTotal = nTRT + nTRTOutliers;
		Int_t		 nBlayerHits =  currElectron->nBLHits();
		Bool_t   expectBlayer = currElectron->expectHitInBLayer();
		Bool_t   debug = false;

		// Int_t nSiOutliers = currElectron->nSCTOutliers() + currElectron->nPixelOutliers();
		// Int_t nPixOutliers = currElectron->nPixelOutliers();  

		Root::TAccept tAccept = m_electronTool2013->accept(Eta_cl_s2, 
													  													 Et_cl_s2,
																											 rHad, 
																											 rHad1, 
																											 Reta, 
																											 w2,
																											 f1,
																											 f3, 
																											 wstot, 
																											 DEmaxs1, 
																											 deltaEta, 
																											 nSi,
																											 nSiDeadSensors, 
																											 nPix, 
																											 nPixDeadSensors,
																											 deltaPhiRescaled,
																											 dpOverp,
																											 rTRT,
																											 nTRTTotal,
																											 nBlayerHits,
																											 expectBlayer,
																											 debug);

		PassElectronID = (Bool_t)tAccept;
	}
	else if (m_dataYear == 2012 && useLikelihood) // or !doMoriond
	{
		Double_t Eta_cl_s2 = currElectron->etas2();
		Double_t Et_cl_s2= currElectron->cl_E() / cosh(Eta_cl_s2) / currElectronObj->getSmear();
		Double_t f3 = currElectron->f3();
		Double_t rHad = currElectron->Ethad()/Et_cl_s2;
		Double_t rHad1 = currElectron->Ethad1()/Et_cl_s2;
		Double_t Reta = currElectron->reta(); 
		Double_t w2 = currElectron->weta2();
		Double_t f1 = currElectron->f1();
		Double_t DEmaxs1 = fabs(currElectron->emaxs1() + currElectron->Emax2()) > 0.
										 ? (currElectron->emaxs1() - currElectron->Emax2()) / (currElectron->emaxs1() + currElectron->Emax2()) 
										 : 0.;
		Double_t deltaEta = currElectron->deltaeta1();
		Double_t d0 = currElectron->trackd0pvunbiased();
		// Use nTRTHTOutliersRatio instead of HTHitsRatio.
		Double_t TRratio = currElectron->TRTHighTOutliersRatio();
		Double_t d0sigma = currElectron->tracksigd0pvunbiased();
		Double_t rphi = currElectron->rphi();
	  Double_t dpOverp  = 0;
		if  (author == 1 || author == 3)
		{
			for (Int_t i = 0; i < currElectron->refittedTrack_LMqoverp().size(); ++i)
			{
				if (currElectron->refittedTrack_author().at(i) == 4)
					dpOverp = 1 - currElectron->trackqoverp() / currElectron->refittedTrack_LMqoverp().at(i);
			}
		}
		Double_t deltaPhiRescaled =  currElectron->deltaphiRescaled();
		Double_t ip = Double_t(getNumVertex(2)); // From UsesEvent class
	
		// For background control regions
		if(useRelaxed) m_electronTool2012->setOperatingPoint(LikeEnum::LooseRelaxed);
		else m_electronTool2012->setOperatingPoint(LikeEnum::Loose);

		Double_t discriminant = m_electronTool2012->calculate(Eta_cl_s2,				
																													Et_cl_s2,
																													f3, 
																													rHad, 
																													rHad1,
																													Reta, 
																													w2, 
																													f1, 
																													DEmaxs1,
																													deltaEta, 
																													d0, 
																													TRratio,
																													d0sigma,
																													rphi,
																													dpOverp ,
																													deltaPhiRescaled,
																													Double_t(ip));

		Int_t    nSi = currElectron->nSiHits();
		Int_t		 nSiOutliers = currElectron->nSCTDeadSensors() + currElectron->nPixelDeadSensors(); 
		Int_t    nPix = currElectron->nPixHits();
		// Use dead pixels instead of outliers.
		Int_t		 nPixDeadSensors = currElectron->nPixelDeadSensors();
		Int_t    nBlayer = currElectron->nBLHits();
		Int_t    nBlayerOutliers = currElectron->nBLayerOutliers();
		Bool_t   expectBlayer = currElectron->expectHitInBLayer();
		if (currElectron->expectHitInBLayer() == -999) expectBlayer = 1;
		Int_t    convBit = currElectron->isEM() & 0x1 << 1; 

		PassElectronID = m_electronTool2012->accept(discriminant,
																								Eta_cl_s2, 
																								Et_cl_s2,
																								nSi,
																								nSiOutliers, 
																								nPix, 
																								nPixDeadSensors,
																								nBlayer, 
																								nBlayerOutliers, 
																								expectBlayer,
																								convBit, 
																								Double_t(ip));
	}
	return PassElectronID;
}

void ElectronCut::init()
{
	m_z0Cut = 10.;

	if (m_dataYear == 2011)
	{
		TString configH4l = "../../ElectronPhotonSelectorTools/python/ConfiguredTElectronIsEMSelectors.py";
		TPython::LoadMacro(configH4l.Data());

		m_electronTool2011 = static_cast<Root::TElectronIsEMSelector*>((void*)TPython::Eval("ConfiguredTElectronIsEMSelector(ROOT.egammaPID.ElectronIDLoosePP,electronPIDmenu.menuH4l2011)")); 
		m_electronTool2011->initialize();
	}
	else if (m_dataYear == 2012 && !useLikelihood) // or doMoriond
	{
		m_electronTool2013 = new Root::TElectronMultiLeptonSelector();
		m_electronTool2013->initialize();
	}
	else if (m_dataYear == 2012 && useLikelihood) // or !doMoriond
	{
		m_electronTool2012 = new Root::TElectronLikelihoodTool();
		m_electronTool2012->setPDFFileName("../../ElectronPhotonSelectorTools/data/ElectronLikelihoodPdfs.root"); // use this root file!
		m_electronTool2012->setOperatingPoint(LikeEnum::Loose);
		m_electronTool2012->initialize();
	}
}