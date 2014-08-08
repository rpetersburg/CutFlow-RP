#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

QuadLepton::QuadLepton(DiLepton *tZ1, DiLepton *tZ2)
{
	setZBosons(tZ1, tZ2);
}

QuadLepton::~QuadLepton()
{

}

void QuadLepton::setZBosons(DiLepton *tZ1, DiLepton *tZ2)
{
	m_z1 = tZ1;
	m_z2 = tZ2;

	if (m_z1->getFlavor() == Flavor::Muon)
	{
		if (m_z1->getFlavor() == Flavor::Muon)
			m_type = QuadType::Mu4;
		else
			m_type = QuadType::Mu2El2;
	}
	else if (m_z1->getFlavor() == Flavor::Electron)
	{
		if (m_z2->getFlavor() == Flavor::Electron)
			m_type = QuadType::El4;
		else
			m_type = QuadType::El2Mu2;
	}

	m_momentum = new TLorentzVector();
	*m_momentum = *m_z1->getMomentumVec() + *m_z2->getMomentumVec();
	m_momentumMain = new TLorentzVector();
	*m_momentumMain = *m_z1->getMomentumMainVec() + *m_z2->getMomentumMainVec();

	m_leptonVec.clear();
	m_leptonLorentzVec.clear();
	m_leptonLorentzMSVec.clear();
	m_leptonLorentzIDVec.clear();

	// Order must be Z1+, Z1-, Z2+, Z2-
	m_leptonVec.push_back(m_z1->getPosLepton());
	m_leptonVec.push_back(m_z1->getNegLepton());
	m_leptonVec.push_back(m_z2->getPosLepton());
	m_leptonVec.push_back(m_z2->getNegLepton());

	vector<ChargedLepton*>::iterator currLepton = m_leptonVec.begin();
	for ( ; currLepton != m_leptonVec.end(); ++currLepton)
	{
		m_leptonLorentzVec.push_back(*(*currLepton)->getMomentumVec());
		m_leptonLorentzMSVec.push_back(*(*currLepton)->getMomentumMSVec());
		m_leptonLorentzIDVec.push_back(*(*currLepton)->getMomentumIDVec());
	}
}

void QuadLepton::fillFSRCorrection(TLorentzVector tFSRMomentum, Bool_t isZ1, Bool_t isZ2, PATCore::ParticleType::Type particleType)
{
	m_fsrMomentum = tFSRMomentum;
	m_fsrLorentzVec.push_back(m_fsrMomentum);
	m_fsrLorentzIDVec.push_back(m_fsrMomentum);
	m_fsrLorentzMSVec.push_back(m_fsrMomentum);

	double energyResolution = m_elRescale->resolution(m_fsrMomentum.E(), m_fsrMomentum.Eta(), particleType, true) * m_fsrMomentum.E();

	TMatrixD photonCovarianceTMatrixD = ZMassConstraint::getCovarianceTMatrixDd0z0PhiThetaPElectron(energyResolution,
																																																	0.000001,
																																																	0.000001,
																																																	0.000001,
																																																	0.000001,
																																																	0.0,
																																																	0.0,
																																																	0.0,
																																																	0.0,
																																																	0.0,
																																																	0.0);

	CLHEP::HepMatrix photonCovarianceHepTMatrixD = ZMassConstraint::getCovarianceMatrixd0z0PhiThetaPElectron(energyResolution,
																																																					 0.000001,
																																																					 0.000001,
																																																					 0.000001,
																																																					 0.000001,
																																																					 0.0,
																																																					 0.0,
																																																					 0.0,
																																																					 0.0,
																																																					 0.0,
																																																					 0.0);

	m_fsrCovMatrixVec.push_back(photonCovarianceTMatrixD);
	m_fsrCovMatrixIDVec.push_back(photonCovarianceTMatrixD);
	m_fsrCovMatrixMSVec.push_back(photonCovarianceTMatrixD);

	m_fsrCovMatrixHepVec.push_back(photonCovarianceHepTMatrixD);
	m_fsrCovMatrixIDHepVec.push_back(photonCovarianceHepTMatrixD);
	m_fsrCovMatrixMSHepVec.push_back(photonCovarianceHepTMatrixD);

	if (isZ1)
	{
		m_z1->setHasFSR(true);
		m_z1->setFSRMomentum(m_fsrMomentum);
		m_z1->setFSRError(photonCovarianceTMatrixD);
		m_z1->setHepFSRError(photonCovarianceHepTMatrixD);
	}
	if (isZ2)
	{
		m_z2->setHasFSR(true);
		m_z2->setFSRMomentum(m_fsrMomentum);
		m_z2->setFSRError(photonCovarianceTMatrixD);
		m_z2->setHepFSRError(photonCovarianceHepTMatrixD);
	}
}

void QuadLepton::clearTrackIsoVecs()
{
	m_trackIso.clear();
	m_trackIsoVal.clear();
}

void QuadLepton::clearCaloIsoVecs()
{
	m_caloIso.clear();
	m_caloIsoVal.clear();
}

void QuadLepton::clearD0SigVecs()
{
	m_d0Sig.clear();
	m_d0SigVal.clear();
}

void QuadLepton::setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale)
{
	m_elRescale = telRescale;
	m_z1->setElRescale(m_elRescale);
	m_z2->setElRescale(m_elRescale);
}

void QuadLepton::setZMass(Double_t tZMass, Int_t muonType)
{
	if (muonType == MuonType::CB) m_zMass = tZMass;
	else if (muonType == MuonType::MS) m_zMassMS = tZMass;
	else if (muonType == MuonType::ID) m_zMassID = tZMass;
}

void QuadLepton::setZMassErr(Double_t tZMassErr, Int_t muonType)
{
	if (muonType == MuonType::CB) m_zMassErr = tZMassErr;
	else if (muonType == MuonType::MS) m_zMassErrMS = tZMassErr;
	else if (muonType == MuonType::ID) m_zMassErrID = tZMassErr;
}

void QuadLepton::setZ1Mass(Double_t tZ1Mass, Int_t muonType)
{
	if (muonType == MuonType::CB) m_z1Mass = tZ1Mass;
}

void QuadLepton::setZ2Mass(Double_t tZ2Mass, Int_t muonType)
{
	if (muonType == MuonType::CB) m_z2Mass = tZ2Mass;
}

void QuadLepton::fillMasses()
{
	m_mass = InvariantMass::invMassCalc(m_leptonLorentzVec);
	m_massID = InvariantMass::invMassCalc(m_leptonLorentzIDVec);
	m_massMS = InvariantMass::invMassCalc(m_leptonLorentzMSVec);	

	for (Int_t i = 0; i < (Int_t)m_leptonLorentzVec.size(); i++)
		m_massUnconstrainedSum = m_massUnconstrainedSum + m_leptonLorentzVec[i];
}

void QuadLepton::fillCovMatrix()
{
	// Order = Z1+, Z1-, Z2+, Z2-
	vector<ChargedLepton*>::iterator leptonItr = m_leptonVec.begin();
	for (; leptonItr != m_leptonVec.end(); ++leptonItr)
	{
		ChargedLepton *lepton = *leptonItr;

		lepton->fillCovMatrix();

		m_covMatrixVec.push_back(lepton->getCovMatrix());
		m_covMatrixHepVec.push_back(lepton->getCovMatrixHep());

		m_covMatrixMSVec.push_back(lepton->getCovMatrix(MuonType::MS));
		m_covMatrixMSHepVec.push_back(lepton->getCovMatrixHep(MuonType::MS));

		m_covMatrixIDVec.push_back(lepton->getCovMatrix(MuonType::ID));
		m_covMatrixIDHepVec.push_back(lepton->getCovMatrixHep(MuonType::ID));

		m_fsrCovMatrixVec = m_covMatrixVec;
		m_fsrCovMatrixHepVec = m_covMatrixHepVec;

		m_fsrCovMatrixMSVec = m_covMatrixMSVec;
		m_fsrCovMatrixMSHepVec = m_covMatrixMSHepVec;

		m_fsrCovMatrixIDVec = m_covMatrixIDVec;
		m_fsrCovMatrixIDHepVec = m_covMatrixIDHepVec;
	}
}

void QuadLepton::fillMassErrors()
{
	m_massErr = MassError::sigmaMassCalc_d0z0PhiThetaP(m_leptonLorentzVec, m_covMatrixVec);
	m_massIDErr = MassError::sigmaMassCalc_d0z0PhiThetaP(m_leptonLorentzIDVec, m_covMatrixIDVec);
	m_massMSErr = MassError::sigmaMassCalc_d0z0PhiThetaP(m_leptonLorentzMSVec, m_covMatrixMSVec);
}

void QuadLepton::fillProductionChannel(vector<Jets*> jetsVec, vector<Muon*> muonVec, vector<Electron*> electronVec)
{
	Bool_t nJets2 = false;
	Bool_t Mjj40_130 = false;
	Bool_t Mjj130 = false;
	Bool_t isolatedLep = false;
	Bool_t hadBDTCut = false;
	Bool_t filledLeadingJet = false;
	Bool_t filledSubleadingJet = false;

	TLorentzVector sumJets;

	// For Jet Information
	if (jetsVec.size() >= 2)
	{
		// Fill the Leading Jet
		Double_t leadingJetPt = -9999;
		for (Int_t i = 0; i < (Int_t)jetsVec.size(); i++)
		{
			if (jetsVec[i]->getMomentumVec()->Pt() > leadingJetPt &&
					jetsVec[i]->getMomentumVec()->Pt() > 25 * 1000)
			{
				m_leadingJet = jetsVec[i];
				leadingJetPt = jetsVec[i]->getMomentumVec()->Pt();
				filledLeadingJet = true;
			}
		}

		if (filledLeadingJet) // Fill the subleading jet
		{
			Double_t subLeadingJetPt = -9999;
			for (Int_t i = 0; i < (Int_t)jetsVec.size(); i++)
			{
				if (jetsVec[i] == m_leadingJet) continue;
				if (jetsVec[i]->getMomentumVec()->Pt() > subLeadingJetPt)
				{
					m_subleadingJet = jetsVec[i];
					subLeadingJetPt = jetsVec[i]->getMomentumVec()->Pt();
					filledSubleadingJet = true;
				}
			}
			// Combine the two leading jets' masses
			sumJets = *m_leadingJet->getMomentumVec() + *m_subleadingJet->getMomentumVec();
			if (sumJets.M() > 130 * 1000) Mjj130 = true;
			else if (sumJets.M() > 40 * 1000 && sumJets.M() < 130 * 1000) Mjj40_130 = true;
		}

		if (filledSubleadingJet) // Fill a third jet
		{
			nJets2 = true; // Only becomes true if leading and subleading jets are found
			Double_t thirdJetPt = -9999;
			for (Int_t i = 0; i < (Int_t)jetsVec.size(); i++)
			{
				if (jetsVec[i] == m_leadingJet || jetsVec[i] == m_subleadingJet) continue;
				if (jetsVec[i]->getMomentumVec()->Pt() > thirdJetPt)
				{
					m_thirdJet = jetsVec[i];
					thirdJetPt = jetsVec[i]->getMomentumVec()->Pt();
				}
			}
		}
	}


	// For BDT output
	m_dijet_invmass = -999 * 1000;
	m_dijet_deltaeta = -999;
	m_leading_jet_pt = -999 * 1000;
	m_leading_jet_eta = -999;
	m_subleading_jet_pt = -999 * 1000;

	m_BDT_discriminant_VBF = -999;
	m_BDT_discriminant_HadVH = -999;

	if (nJets2)
	{
		m_dijet_invmass = sumJets.M();
		m_dijet_deltaeta = fabs(m_leadingJet->getMomentumVec()->Eta() - m_subleadingJet->getMomentumVec()->Eta());
		m_leading_jet_pt = m_leadingJet->getMomentumVec()->Pt();
		m_leading_jet_eta = m_leadingJet->getMomentumVec()->Eta();
		m_subleading_jet_pt = m_subleadingJet->getMomentumVec()->Pt();

		// computing HadVH and VBF BDT discriminant and filling the variables
		BDT_discriminant_VBF = CategoriesDiscriminantTool->Get_VBFDiscriminant_Output();
		BDT_discriminant_HadVH = CategoriesDiscriminantTool->Get_HadVHDiscriminant_Output();

		if (CategoriesDiscriminantTool->Pass_HadVHDiscriminant())
			hadBDTCut = true;
	}
	vector<ChargedLepton *> extraLep;
	// For isolated Lepton
	if ((muonVec.size() + electronVec.size()) > 4)
	{
		for (Int_t i = 0; i < (Int_t)muonVec.size(); i++)
		{
			if (isGoodExtraLepton(higgs, muonVec[i]))
			{
				//higgs->SetProductionChannel(productionChannel::VH);
				//return;
				isolatedLep = true;
				fillExtraLepParent(muonVec[i]);
				extraLep.push_back(muonVec[i]);
			}
		}
		for (Int_t i = 0; i < (Int_t)electronVec.size(); i++)
		{
			if (isGoodExtraLepton(higgs, electronVec[i]))
			{
				//higgs->SetProductionChannel(productionChannel::VH);
				//return;
				isolatedLep = true;
				fillExtraLepParent(electronVec[i]);
				extraLep.push_back(electronVec[i]);
			}
		}

	}

	ChargedLepton *leadingExtraLep = 0;
	ChargedLepton *subleadingExtraLep = 0;
	Double_t lep_leadingPt = -999999;
	Double_t sublep_leadingPt = -999999;
	for (Int_t i = 0; i < (Int_t)extraLep.size(); i++)
	{
		if (extraLep[i]->getMomentumVec()->Pt() > lep_leadingPt)
		{
			sublep_leadingPt = lep_leadingPt;
			subleadingExtraLep = leadingExtraLep;

			leadingExtraLep = extraLep[i];
			lep_leadingPt = extraLep[i]->getMomentumVec()->Pt();
		}
		else if (extraLep[i]->getMomentumVec()->Pt() > sublep_leadingPt)
		{
			subleadingExtraLep = extraLep[i];
			sublep_leadingPt = extraLep[i]->getMomentumVec()->Pt();
		}
	}

	// For ggF catergorization
	//higgs->SetProductionChannel(productionChannel::ggF);

	//Setting the categories
	if (nJets2 && Mjj130) m_productionChannel = ProductionChannel::VBF;
	else if (nJets2 && Mjj40_130 && hadBDTCut) m_productionChannel = ProductionChannel::VHHad;
	else if (isolatedLep) m_productionChannel = ProductionChannel::VHLep;
	else m_productionChannel = ProductionChannel::ggF;

	//Filling variables for the tree
	higgs->leadingJet = leadingJet;
	higgs->subLeadingJet = subLeadingJet;
	higgs->thirdJet = thirdJet;
	higgs->n_jets = jetsVec.size();
	higgs->dijet_invmass = dijet_invmass / 1000;
	higgs->dijet_deltaeta = dijet_deltaeta;
	higgs->leading_jet_pt = leading_jet_pt / 1000;
	higgs->leading_jet_eta = leading_jet_eta;
	higgs->subleading_jet_pt = subleading_jet_pt / 1000;
	higgs->BDT_discriminant_VBF = BDT_discriminant_VBF;
	higgs->BDT_discriminant_HadVH = BDT_discriminant_HadVH;
	higgs->leadingExtraLep = leadingExtraLep;
	higgs->subleadingExtraLep = subleadingExtraLep;

	extraLep.clear();
}