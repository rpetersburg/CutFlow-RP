#include "CutFlow4Lep/Smear/MuonSmear4Lep.h"

MuonSmear4Lep::MuonSmear4Lep(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting) : Smear(tEvent), m_pileupReweighting(tPileupReweighting)
{
	initializeSmearObj();
	initializeResolutionAndMomentumScaleFactors();
	initializeScaleFactors();
}

MuonSmear4Lep::~MuonSmear4Lep()
{

}

void MuonSmear4Lep::muonUncertaintyCorrection(D3PDReader::MuonD3PDObjectElement currMuon)
{
		// muon type, =1 for combined muons, =2 for calorimeter and segment tagged muons, =3 for stand-alone muons 
		Int_t muonType = 2;
		
		TLorentzVector muLorentz;
		muLorentz.SetPtEtaPhiE(currMuon.pt(), currMuon.eta(), currMuon.phi(), currMuon.E());

		Double_t MuonErrSF = m_muonResoMomScaleFactors->getResolutionScaleFactor(muLorentz, muonType);

		currMuon.cov_qoverp_exPV() *= MuonErrSF * MuonErrSF;
		currMuon.cov_d0_qoverp_exPV() *= MuonErrSF;
		currMuon.cov_z0_qoverp_exPV() *= MuonErrSF;
		currMuon.cov_phi_qoverp_exPV() *= MuonErrSF;
		currMuon.cov_theta_qoverp_exPV() *= MuonErrSF;

		TLorentzVector muLorentzME;
    muLorentzME.SetPtEtaPhiM(currMuon.me_pt, currMuon.me_eta, currMuon.me_phi(), pdgMuMass);

		Double_t MuonErrSFME = m_muonResoMomScaleFactors->getResolutionScaleFactor(muLorentzME, muonType);

		currMuon.me_cov_qoverp_exPV() *= MuonErrSFME * MuonErrSFME;
		currMuon.me_cov_d0_qoverp_exPV() *= MuonErrSFME;
		currMuon.me_cov_z0_qoverp_exPV() *= MuonErrSFME;
		currMuon.me_cov_phi_qoverp_exPV() *= MuonErrSFME;
		currMuon.me_cov_theta_qoverp_exPV() *= MuonErrSFME;

		TLorentzVector muLorentzID;
		muLorentzID.SetPtEtaPhiM(currMuon.id_pt, currMuon.id_eta, currMuon.id_phi(), pdgMuMass);

		Double_t MuonErrSFID = m_muonResoMomScaleFactors->getResolutionScaleFactor(muLorentzID, muonType);

		currMuon.id_cov_qoverp_exPV() *= MuonErrSFID * MuonErrSFID;
		currMuon.id_cov_d0_qoverp_exPV() *= MuonErrSFID;
		currMuon.id_cov_z0_qoverp_exPV() *= MuonErrSFID;
		currMuon.id_cov_phi_qoverp_exPV() *= MuonErrSFID;
		currMuon.id_cov_theta_qoverp_exPV() *= MuonErrSFID;
}

void MuonSmear4Lep::initializeSmearObj()
{
	if (m_dataYear == 2011)
		m_smearTool = new MuonSmear::SmearingClass("Data11", "staco", "q_pT", "Re117", "../../../MuonMomentumCorrections/share/");	
	else if (m_dataYear == 2012)
		m_smearTool = new MuonSmear::SmearingClass("Data12", "staco", "q_pT", "Re117.2Sum13", "../../../MuonMomentumCorrections/share/");

	m_smearTool->UseScale(1);
	m_smearTool->UseImprovedCombine();	
}

void MuonSmear4Lep::initializeResolutionAndMomentumScaleFactors()
{
	string scaleFactorFile;
	if (m_dataYear == 2011 && m_isMC)
		scaleFactorFile = "MC11_smearing";
	else if (m_dataYear == 2011 && !m_isMC)
		scaleFactorFile = "data2011";
	else if (m_dataYear == 2012 && m_isMC)
		scaleFactorFile = "MC12_smearing";
	else if (m_dataYear == 2012 && !m_isMC)
		scaleFactorFile = "data2012";

	m_muonResoMomScaleFactors = new Analysis::MuonResolutionAndMomentumScaleFactors("../../../MuonMomentumCorrections/share/final_scale_factors_" + scaleFactorFile + ".txt");
}

void MuonSmear4Lep::initializeScaleFactors()
{
	string directory("../../../MuonEfficiencyCorrections/share/");
	string unit("MeV");
	string muonTypeStaco;
	string muonTypeStacoSA;
	string muonTypeCaloMu;

	Analysis::AnalysisMuonConfigurableScaleFactors::Configuration config = Analysis::AnalysisMuonConfigurableScaleFactors::AverageOverPeriods;
	Analysis::AnalysisMuonConfigurableScaleFactors::Configuration configSA;

	vector<string> periodLetter;
	vector<Double_t> luminosity;

	if (m_dataYear == 2011)
	{
		muonTypeStaco = "STACO_CB_plus_ST_2011_SF.txt.gz";
		muonTypeStacoSA = "STACOHighEta.txt.gz";
		muonTypeCaloMu = "CaloTag_2011_SF.txt.gz";
		configSA = Analysis::AnalysisMuonConfigurableScaleFactors::AverageOverPeriods;

		const Int_t len = 11;
		string periodLetterArray[len] = {"B", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"};
		periodLetter.assign(periodLetterArray, periodLetterArray+len);
		Double_t luminosityArray[len] = {m_pileupReweighting->GetIntegratedLumi(177986,178109),
																m_pileupReweighting->GetIntegratedLumi(179710,180481),
																m_pileupReweighting->GetIntegratedLumi(180614,180776),
																m_pileupReweighting->GetIntegratedLumi(182013,182519),
																m_pileupReweighting->GetIntegratedLumi(182726,183462),
																m_pileupReweighting->GetIntegratedLumi(183544,184169),
																m_pileupReweighting->GetIntegratedLumi(185353,186493),
																m_pileupReweighting->GetIntegratedLumi(186516,186755),
																m_pileupReweighting->GetIntegratedLumi(186873,187815),
																m_pileupReweighting->GetIntegratedLumi(188902,190343),
																m_pileupReweighting->GetIntegratedLumi(190503,191933)};
		luminosity.assign(luminosityArray, luminosityArray+len);
		Double_t totalLuminosity = m_pileupReweighting->GetIntegratedLumi();
	}
	else if (m_dataYear == 2012)
	{
		muonTypeStaco = "STACO_CB_plus_ST_2012_SF.txt.gz";
		muonTypeStacoSA = "STACO_CB_plus_ST_2012_SFms.txt.gz";
		muonTypeCaloMu = "CaloTag_2012_SF.txt.gz";
		configSA = Analysis::AnalysisMuonConfigurableScaleFactors::AverageOverPeriods;

		const Int_t len = 13;
		string periodLetterArray[len] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"};
		periodLetter.assign(periodLetterArray, periodLetterArray+len);
		Double_t luminosityArray[len] = {m_pileupReweighting->GetIntegratedLumi(200804,201556),
      													m_pileupReweighting->GetIntegratedLumi(202660,205113),
      													m_pileupReweighting->GetIntegratedLumi(206248,207397),
      													m_pileupReweighting->GetIntegratedLumi(207447,209025),
      													m_pileupReweighting->GetIntegratedLumi(209074,210308),
      													0.0,
																m_pileupReweighting->GetIntegratedLumi(211522,212272),
      													m_pileupReweighting->GetIntegratedLumi(212619,213359),
      													m_pileupReweighting->GetIntegratedLumi(213431,213819),
      													m_pileupReweighting->GetIntegratedLumi(213900,215091),
      													0.0,
      													m_pileupReweighting->GetIntegratedLumi(215414,215643),
																m_pileupReweighting->GetIntegratedLumi(216399,216432)};
		luminosity.assign(luminosityArray, luminosityArray+len);
	}
	m_stacoCSF = new Analysis::AnalysisMuonConfigurableScaleFactors(directory, muonTypeStaco, unit, config);
	m_stacoSACSF = new Analysis::AnalysisMuonConfigurableScaleFactors(directory, muonTypeStaco, unit, configSA);
	m_caloMuCSF = new Analysis::AnalysisMuonConfigurableScaleFactors(directory, muonTypeCaloMu, unit, config);
	for (Int_t i = 0; i < periodLetter.size(); i++)
	{
		m_stacoCSF->addPeriod(periodLetter[i], luminosity[i]);
		m_stacoSACSF->addPeriod(periodLetter[i], luminosity[i]);
		m_caloMuCSF->addPeriod(periodLetter[i], luminosity[i]);
	}
	m_stacoCSF->Initialise();
	m_stacoSACSF->Initialise();
	m_caloMuCSF->Initialise();
}