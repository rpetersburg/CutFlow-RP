#include "CutFlow4Lep/Corrections/FSRCorrection.h"

FSRCorrection::FSRCorrection(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, ElectronSmear *tElectronSmearObj)
	: Correction(tEvent), m_quadLepton(tQuadLepton), m_electronSmearObj(tElectronSmearObj)
{
	m_leptonVec = m_quadLepton->getLeptons();
}

FSRCorrection::~FSRCorrection()
{

}

void FSRCorrection::executeCorrection()
{
	m_quadLepton->setFSRType(FSRType::noFSR);

	vector<FsrPhotons::FsrCandidate> candidateVec;
	fillCandidateVec(candidateVec);
	
	// Executing these boolean functions will fill FSR correction in QuadLepton
	Bool_t passFSRCollinear = passedFSRCollinear(candidateVec);
	Bool_t passFSRFar = false;
	if (!passFSRCollinear)
		passFSRFar = passedFSRFar(candidateVec);

	m_quadLepton->setFSRMass(InvariantMass::invMassCalc(m_quadLepton->getFSRLorentzVec()));
	m_quadLepton->setFSRMassError(MassError::sigmaMassCalc_d0z0PhiThetaP(m_quadLepton->getFSRLorentzVec(),
																																			 m_quadLepton->getFSRCovMatrixVec()));

	m_quadLepton->setFSRZ1Mass(zMassWithFSR(m_quadLepton->getZ1()));
	m_quadLepton->setFSRZ2Mass(zMassWithFSR(m_quadLepton->getZ2()));

	vector<TLorentzVector> fsrLorentzVec = m_quadLepton->getFSRLorentzVec();
	for (Int_t i = 0; i < (Int_t)fsrLorentzVec.size(); i++)
		m_quadLepton->setFSRSum(m_quadLepton->getFSRSum() + fsrLorentzVec[i]);

	fsrLorentzVec.clear();
}

void FSRCorrection::fillCandidateVec(vector<FsrPhotons::FsrCandidate> candidateVec)
{
	vector<float> *ph_EtCone40_corr;
	fill_ph_EtCone40_corr(ph_EtCone40_corr);

	vector<float> *el_bfEP_cl_pt;
	fill_el_bfEP_cl_pt(el_bfEP_cl_pt);

	vector<ChargedLepton*>::iterator leptonItr = m_leptonVec.begin();
	Int_t i = -1;
	for ( ; leptonItr != m_leptonVec.end(); ++leptonItr)
	{
		i++;
		ChargedLepton *lepton = *leptonItr;

		if (lepton->getFlavor() == Flavor::Muon)
		{
			D3PDReader::MuonD3PDObjectElement *muon = lepton->getMuon();
			FsrPhotons fsrPhotons;
			FsrPhotons::FsrCandidate fsrCandidate;

			if (lepton->getType() == LeptonType::MuonCalo)
				candidateVec.push_back(fsrCandidate);
			else if (fsrPhotons.getFsrPhotons(muon->eta(),
																				muon->phi(),
																				muon->id_theta(),
																				muon->id_phi(),
																				m_event->ph.eta(),
																				m_event->ph.phi(),
																				m_event->ph.Et(),
																				m_event->ph.f1(),
																				ph_EtCone40_corr,
																				m_event->ph.author(),
																				m_currElectron->cl_eta(),
																				m_currElectron->cl_phi(),
																				el_bfEP_cl_pt,
																				m_currElectron->f1(),
																				m_currElectron->tracktheta(),
																				m_currElectron->trackphi(),
																				fsrCandidate, // Fills FSRCandidate
																				m_event->ph.tight()))
			{
				cout << "Muon " << i << " Muon eta: " << muon->eta() << " Muon Phi" << muon->phi() << endl;
    		cout << "Found FSR index, dR, Et, f1, cont " << fsrCandidate.index << " " << fsrCandidate.deltaR
    		     << " " << fsrCandidate.Et << " " <<  fsrCandidate.f1 << " " << fsrCandidate.container << std::endl;
				cout << "Muon info: eta: " << muon->eta() << " phi: " << muon->phi() << " id_theta: "
					   << muon->id_theta() << " id_phi: " << muon->id_phi() << endl;
				cout << "-------------------" << endl;
				candidateVec.push_back(fsrCandidate);
			}
			else
				candidateVec.push_back(fsrCandidate);
		}
		else if (lepton->getFlavor() == Flavor::Electron)
		{
			D3PDReader::ElectronD3PDObjectElement *electron = lepton->getElectron();
			FsrPhotons fsrPhotons;
			FsrPhotons::FsrCandidate fsrCandidate;
			if (fsrPhotons.getFsrPhotons(electron->tracketa(),
																	 electron->trackphi(),
																	 m_event->ph.eta(),
																	 m_event->ph.phi(),
																	 m_event->ph.Et(),
																	 m_event->ph.f1(),
																	 ph_EtCone40_corr,
																	 m_event->ph.author(),
																	 m_event->ph.tight(),
																	 fsrCandidate)) // Fills FSRCandidate
			{
				cout << "electron " << i << endl;
    		cout << "Found FSR index, dR, Et, f1, cont " << fsrCandidate.index << " " << fsrCandidate.deltaR
    		     << " " << fsrCandidate.Et << " " <<  fsrCandidate.f1 << " " << fsrCandidate.container << std::endl;
				cout << "-------------------" << endl;
			}
			candidateVec.push_back(fsrCandidate);
		}
	}

	if (candidateVec.size() != 4) // Sanity check
		cout << "FSRCorrection::executeCorrection: Don't have 4 FSR candidates" << endl;
}

void FSRCorrection::fill_ph_EtCone40_corr(vector<float> *ph_EtCone40_corr)
{
	double caloIsoCorr = 0;

	for (Int_t i = 0; i < m_event->ph.n(); i++)
	{
		if (m_dataYear == 2011)
		{
			caloIsoCorr = CaloIsoCorrection::GetNPVCorrectedIsolation(getNumVertex(2),
																																m_event->ph[i].etas2(),
																																40,
																																m_isMC,
																																m_event->ph[i].Etcone40(),
																																CaloIsoCorrection::PHOTON,
																																CaloIsoCorrection::REL17);
			ph_EtCone40_corr->push_back(caloIsoCorr);
		}
		else if (m_dataYear == 2012)
		{
			Double_t clusterE = m_event->ph[i].cl_E();
			clusterE = m_event->ph[i].cl_E_unsmeared; // if (doCorr)
			caloIsoCorr = CaloIsoCorrection::GetPtEDCorrectedTopoIsolation(m_event->ph[i].ED_median(),
																																		 clusterE,
																																		 m_event->ph[i].etas2(),
																																		 m_event->ph[i].etap(),
																																		 m_event->ph[i].cl_eta(),
																																		 40,
																																		 m_isMC,
																																		 m_event->ph[i].topoEtcone40(),
																																		 false,
																																		 CaloIsoCorrection::PHOTON,
																																		 CaloIsoCorrection::REL17_2);
			ph_EtCone40_corr->push_back(caloIsoCorr);
		}
	}
}

void FSRCorrection::fill_el_bfEP_cl_pt(vector<float> *el_bfEP_cl_pt)
{
	for (Int_t i = 0; i < m_currElectron->n(); i++)
		el_bfEP_cl_pt->push_back(m_electronSmearObj->getbfEP_cl_pT[i]);
}

Bool_t FSRCorrection::passedFSRCollinear(vector<FsrPhotons::FsrCandidate> candidateVec)
{
	Bool_t passFSRCollinear = false;
	// Collinear (only 4mu or 2mu2el)
	if (m_quadLepton->getType == QuadType::Mu4 || m_quadLepton->getType() == QuadType::Mu2El2)
	{
		Double_t maxEt = -99999;
		FsrPhotons::FsrCandidate fsrCandidate;
		Bool_t isFSR = false;
		
		for (Int_t i = 0; i < 2; i++) // Use first two FSR candidates and find one with highest FSR ET
		{
			FsrPhotons::FsrCandidate tempCandidate = candidateVec[i];
			if (tempCandidate.index != -1 && tempCandidate.Et > maxEt && tempCandidate.deltaR <= 0.15)
			{
				maxEt = tempCandidate.Et;
				fsrCandidate = tempCandidate;
				isFSR = true;
			}
		}
		if (isFSR &&
				m_quadLepton->getZ1()->getMomentumVec()->M() < 89 * 1000 &&
				m_quadLepton->getZ1()->getMomentumVec()->M() > 66 * 1000)
		{
			TLorentzVector momFSR;
			momFSR.SetPtEtaPhiM(fsrCandidate.Et, fsrCandidate.eta, fsrCandidate.phi, 0.0);

			// Finding the combined mass of the Z Bosons with the FSR Correction
			Double_t combMass = (*m_quadLepton->getZ1()->getPosLepton()->getMomentumVec() +
													 *m_quadLepton->getZ1()->getNegLepton()->getMomentumVec() +
													 momFSR).M();

			if (combMass < 100 * 1000)
			{
				passFSRCollinear = true;
				m_quadLepton->setFSRType(FSRType::collFSRZ1mumu);

				// Modify the QuadLepton Object with FSR Correction
				m_quadLepton->fillFSRCorrection(momFSR, true, false, particleType(fsrCandidate));
			}
		}
	}
	return passFSRCollinear;
}

Bool_t FSRCorrection::passedFSRFar(vector<FsrPhotons::FsrCandidate> candidateVec)
{
	FsrPhotons::FsrCandidate candidate;

	// Loop over all candidates and find the highest ET cluster for FSR
	Double_t maxET = -99999;
	Bool_t isFSR = false;
	for (Int_t i = 0; i < (Int_t)candidateVec.size(); i++)
	{
		FsrPhotons::FsrCandidate tempCandidate = candidateVec[i];

		if (tempCandidate.index != -1 && tempCandidate.Et > maxET && tempCandidate.deltaR > 0.15)
		{
			maxET = tempCandidate.Et;
			candidate = tempCandidate;
			isFSR = true;
		}
	}

	Bool_t passFSRFar = false;
	if (isFSR) // Only continue if max FSR found
	{
		TLorentzVector fsrMomentum;
		fsrMomentum.SetPtEtaPhiM(candidate.Et, candidate.eta, candidate.phi, 0.0);

		Bool_t deltaRCut = false;
		vector<ChargedLepton*>::iterator leptonItr = m_leptonVec.begin();
		for ( ; leptonItr != m_leptonVec.end(); ++leptonItr)
			if (fsrMomentum.DeltaR(*(*leptonItr)->getMomentumVec()) < 0.15) deltaRCut = true;

		if (!deltaRCut)
		{
			Double_t deltaZ1Mass = 999999;
			Double_t deltaZ2Mass = 999999;
			// If the DiLepton mass is less than 81 GeV, find how much the FSR moves the mass
			// closer to the Z pole mass... 100 GeV limit there as well
			if (m_quadLepton->getZ1()->getMomentumVec()->M() < 81 * 1000)
			{
				TLorentzVector combinedLorentz = *m_quadLepton->getZ1()->getMomentumVec() + fsrMomentum;
				if (combinedLorentz.M() < 100 * 1000) deltaZ1Mass = fabs(combinedLorentz.M() - pdgZMass);
			}
			if (m_quadLepton->getZ2()->getMomentumVec()->M() < 81 * 1000)
			{
				TLorentzVector combinedLorentz = *m_quadLepton->getZ2()->getMomentumVec() + fsrMomentum;
				if (combinedLorentz.M() < 100 * 1000) deltaZ2Mass = fabs(combinedLorentz.M() - pdgZMass);
				if (m_quadLepton->getMomentumVec()->M() < 190 * 10000) deltaZ2Mass = 999999;
			}

			if (deltaZ1Mass < deltaZ2Mass && deltaZ1Mass != 999999)
			{
				m_quadLepton->fillFSRCorrection(fsrMomentum, true, false, particleType(candidate));
				passFSRFar = true;
				m_quadLepton->setFSRType(FSRType::farFSRZ1);
			}
			else if (deltaZ2Mass < deltaZ1Mass && deltaZ2Mass != 999999)
			{
				m_quadLepton->fillFSRCorrection(fsrMomentum, false, true, particleType(candidate));
				passFSRFar = true;
				m_quadLepton->setFSRType(FSRType::farFSRZ2);
			}
		}
	}
	return passFSRFar;
}

PATCore::ParticleType::Type FSRCorrection::particleType(FsrPhotons::FsrCandidate candidate)
{
	PATCore::ParticleType::Type particleType = PATCore::ParticleType::Electron;
	if (candidate.container == "electron") particleType = PATCore::ParticleType::Electron;
	else if (candidate.container == "photon")
	{
		if (m_event->ph[candidate.index].isConv())
			particleType = PATCore::ParticleType::ConvertedPhoton;
		else particleType = PATCore::ParticleType::UnconvertedPhoton;
	}
	return particleType;
}

Double_t FSRCorrection::zMassWithFSR(DiLepton *diLepton)
{
	Double_t mass = diLepton->getMomentumVec()->M();
	if (diLepton->getHasFSR())
		mass = (*diLepton->getMomentumVec() + m_quadLepton->getFSRMomentum()).M();
}