#include "CutFlow4Lep/Corrections/JetCalibration.h"

JetCalibration::JetCalibration(D3PDReader::Event *tEvent, Int_t tCurrMCCollection)
	: Correction(tEvent), m_currMCCollection(tCurrMCCollection)
{

}

JetCalibration::~JetCalibration()
{

}

void JetCalibration::executeCorrection()
{
	Double_t mu = m_event->eventinfo.averageIntPerXing();
	if (m_isMC && m_dataYear == 2012)
		mu = (m_event->eventinfo.lbn() == 1 && int(mu + 0.5) == 1) ? 0. : mu;

	Double_t rhoKt = -1;
	if (m_dataYear == 2012) rhoKt = m_event->Eventshape.rhoKt4EM();

	Int_t nVertex = 0;
	for (Int_t i = 0; i < m_event->vxp.n(); i++)
		if (m_event->vxp[i].trk_n() >= 2) nVertex++;

	D3PDReader::JetD3PDObject *jet = &(m_event->jet_akt4topoem);

	for (Int_t i = 0; i < jet->n(); i++)
	{
		D3PDReader::JetD3PDObjectElement currJet = (*jet)[i];

		TLorentzVector calibratedJet;
		Double_t E, pT, eta, eta_det, phi, m, Ax, Ay, Az, Ae, rho;
		if (m_dataYear == 2012)
			calibratedJet = m_jetCalibrationTool->ApplyJetAreaOffsetEtaJES(currJet.emscale_E(),
																																		 currJet.emscale_eta(),
																																		 currJet.emscale_phi(),
																																		 currJet.emscale_m(),
																																		 currJet.ActiveAreaPx(),
																																		 currJet.ActiveAreaPy(),
																																		 currJet.ActiveAreaPz(),
																																		 currJet.ActiveAreaE(),
																																		 rhoKt,
																																		 mu,
																																		 nVertex);
		else if (m_dataYear == 2011)
			calibratedJet = m_jetCalibrationTool->ApplyOffsetEtaJES(currJet.emscale_E(),
																														  currJet.emscale_eta(),
																															currJet.EtaOrigin(),
																															currJet.PhiOrigin(),
																															currJet.MOrigin(),
																															mu,
																															nVertex);

		currJet.E() = calibratedJet.E();
		currJet.pt() = calibratedJet.Pt();
		currJet.eta() = calibratedJet.Eta();
		currJet.phi() = calibratedJet.Phi();
		currJet.m() = calibratedJet.M();
	}
}

void JetCalibration::initializeCorrectionTool()
{
	TString jetAlgorithm = "AntiKt4TopoEM";
	TString jetCalibrationConfigFile = "";
	if (m_dataYear == 2011)
	{
		jetCalibrationConfigFile = "../../ApplyJetCalibration/data/CalibrationConfigs/InsituJES_2011_Preliminary.config";
	}
	else if (m_dataYear == 2012)
	{
		if(!m_isMC) 	 
			jetCalibrationConfigFile = "../../ApplyJetCalibration/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_Jan13.config";
		else if	(m_currMCCollection == MCCollection::MC12a) 
			jetCalibrationConfigFile = "../../ApplyJetCalibration/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_Jan13.config";
		else if	(m_currMCCollection == MCCollection::MC12b) 
			jetCalibrationConfigFile = "../../ApplyJetCalibration/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_MC12b_Sep23.config";
		else if	(m_currMCCollection == MCCollection::MC12c) 
			jetCalibrationConfigFile = "../../ApplyJetCalibration/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_MC12b_Sep23.config";
	}
	m_jetCalibrationTool = new JetAnalysisCalib::JetCalibrationTool(jetAlgorithm, jetCalibrationConfigFile, !m_isMC);
}