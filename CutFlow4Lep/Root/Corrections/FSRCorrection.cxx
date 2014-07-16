#include "CutFlow4Lep/Corrections/FSRCorrection.h"

FSRCorrection::FSRCorrection(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, ElectronSmear *tElectronSmearObj)
	: Correction(tEvent), m_quadLepton(tQuadLepton), m_electronSmearObj(tElectronSmearObj)
{

}

FSRCorrection::~FSRCorrection()
{

}

void FSRCorrection::executeCorrection()
{
	m_quadLepton->setFSRType(FSRType::noFSR);

	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();
	vector<FsrPhotons::FsrCandidate> candVec;

	vector<float> *ph_EtCone40_corr;
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

	D3PDReader::ElectronD3PDObject *currElectron;

	if (m_dataYear == 2011) currElectron = &(m_event->el_GSF);
	else if (m_dataYear == 2012) currElectron = &(m_event->el);

	// Get the corrected electron cl_pt values from the smear object
	vector<float> *el_bfEP_cl_pt;
	for (Int_t i = 0; i < currElectron->n(); i++)
		el_bfEP_cl_pt->push_back(m_electronSmearObj->getbfEP_cl_pT[i]);

	vector<ChargedLepton*>::iterator leptonItr = leptonVec.begin();
	Int_t i = -1;
	for ( ; leptonItr != leptonVec.end(); ++leptonItr)
	{
		i++;
		ChargedLepton *lepton = *leptonItr;

		if (lepton->getFlavor() == Flavor::Muon)
		{
			D3PDReader::MuonD3PDObjectElement *muon = lepton->getMuon();
			FsrPhotons fsrPhotons;
			FsrPhotons::FsrCandidate fsrCandidate;

			if (lepton->getType() == LeptonType::MuonCalo)
				candVec.push_back(fsrCandidate);
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
																				currElectron->cl_eta(),
																				currElectron->cl_phi(),
																				el_bfEP_cl_pt,
																				currElectron->f1(),
																				currElectron->tracktheta(),
																				currElectron->trackphi(),
																				fsrCandidate,
																				m_event->ph.tight()))
			{
				cout << "Muon " << i << " Muon eta: " << muon->eta() << " Muon Phi" << muon->phi() << endl;
    		cout << "Found FSR index, dR, Et, f1, cont " << fsrCandidate.index << " " << fsrCandidate.deltaR
    		     << " " << fsrCandidate.Et << " " <<  fsrCandidate.f1 << " " << fsrCandidate.container << std::endl;
				cout << "Muon info: eta: " << muon->eta() << " phi: " << muon->phi() << " id_theta: "
					   << muon->id_theta() << " id_phi: " << muon->id_phi() << endl;
				cout << "-------------------" << endl;
				candVec.push_back(fsrCandidate);
			}
			else
				candVec.push_back(fsrCandidate);
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
																	 fsrCandidate))
			{
				cout << "electron " << i << endl;
    		cout << "Found FSR index, dR, Et, f1, cont " << fsrCandidate.index << " " << fsrCandidate.deltaR
    		     << " " << fsrCandidate.Et << " " <<  fsrCandidate.f1 << " " << fsrCandidate.container << std::endl;
				cout << "-------------------" << endl;
			}
			candVec.push_back(fsrCandidate);
		}
	}
	// Clearing up memory
	el_bfEP_cl_pt->clear();
	ph_EtCone40_corr->clear();
	delete el_bfEP_cl_pt;
	delete ph_EtCone40_corr;

	if (candVec.size() != 4) // Sanity check
		cout << "FSRCorrection::executeCorrection: Don't have 4 FSR candidates" << endl;

	Bool_t passFSRCollinear = false;
	Bool_t passFSRFar = false;

	// Collinear (only 4mu or 2mu2el)
	if (m_quadLepton->getType == QuadType::Mu4 || m_quadLepton->getType() == QuadType::Mu2El2)
	{
		Double_t maxEt = -99999;
		FsrPhotons::FsrCandidate fsrCandidate;
		Bool_t isFSR = false;
		// Use first two FSR and find highest cluster Et fsr
		for (Int_t i = 0; i < 2; i++)
		{
			FsrPhotons::FsrCandidate tempCandidate = candVec[i];

			if (tempCandidate.index != -1 && tempCandidate.Et > maxEt && tempCandidate.deltaR <= 0.15)
			{
				maxEt = tempCandidate.Et;
				fsrCandidate = tempCandidate;
				isFSR = true;
			}
		}
		if (isFSR && m_quadLepton->getZ1()->getMomentumVec()->M() < 89 * 1000 && m_quadLepton->getZ1()->getMomentumVec()->M() > 66 * 1000)
		{
			TLorentzVector momFSR;
			momFSR.SetPtEtaPhiM(fsrCandidate.Et, fsrCandidate.eta, fsrCandidate.phi, 0.0);

			Double_t combMass = (*m_quadLepton->getZ1()->getPosLepton()->getMomentumVec() +
													 *m_quadLepton->getZ1()->getNegLepton()->getMomentumVec() +
													 momFSR).M();
			if (combMass < 100 * 1000)
			{
				// Set PATCore Particle Type
				PATCore::ParticleType::Type particleType;
				if (fsrCandidate.container == "electron") particleType = PATCore::ParticleType::Electron;
				else if (fsrCandidate.container == "photon")
				{
					if (m_event->ph[fsrCandidate.index].isConv()) particleType = PATCore::ParticleType::ConvertedPhoton;
					else particleType = PATCore::ParticleType::UnconvertedPhoton;
				}
				// Modify the QuadLepton Object with FSR Correction
				m_quadLepton->fillFSRCorrection(momFSR, true, false, particleType);
				passFSRCollinear = true;
				m_quadLepton->setFSRType(FSRType::collFSRZ1mumu);
			}
		}
	}
}