#include "CutFlow4Lep/Smear/MuonCaloSmear.h"

MuonCaloSmear::MuonCaloSmear(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting)
	: MuonSmear4Lep(tEvent, tPileupReweighting)
{

}

MuonCaloSmear::~MuonCaloSmear()
{

}

void MuonCaloSmear::executeSmear()
{
	vector<Double_t> smearVal;

	// Smear the calorimeter muons
	for (Int_t i = 0; i < m_muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*m_muon)[i];

		Int_t isCaloMuonId = currMuon.isCaloMuonId();
		Double_t phi = currMuon.phi();
		Double_t E = currMuon.E();
		Double_t charge = currMuon.charge();
		Double_t eta = currMuon.eta();
		Double_t pT = currMuon.pt();
		Double_t p = 0;

		Double_t ptMs;
		if (isCaloMuonId) ptMs = pT;
		else ptMs = sin(currMuon.me_theta()) / fabs(currMuon.me_qoverp());

		Double_t smear = 1.;
		if (m_isMC)
		{
			m_smearObj->SetSeed(m_eventNumber, i);

			// Smearing from Simulation
			if (isCaloMuonId)
			{
				m_smearObj->Event(pT, eta, "ID", charge, phi);

				Double_t smearedpTID = m_smearObj->pTID();

				smear = smearedpTID / pT;
				pT = smearedpTID;
				p = pT * sin(currMuon.id_theta());
				E = E * smear;

				currMuon.id_pt = smearedpTID;
				currMuon.me_pt = smearedpTID;
			}
			else cout << "MuonSmear4Lep::executeSmear(): Calo Type Mismatch" << endl;
			
			// Muon Efficiency
			Double_t muonEff = 1.;
			Double_t pTCutCalo;
			if (m_dataYear == 2011) pTCutCalo = 5 * 1000;
			else if (m_dataYear == 2012) pTCutCalo = 15 * 1000;

			if (pT >= pTCutCalo && fabs(eta) <= 0.4)
			{
				TLorentzVector lorentzVector;
				lorentzVector.SetPtEtaPhiE(pT, eta, phi, E);

				if (isCaloMuonId && m_dataYear == 2011) muonEff = m_caloMuCSF->scaleFactor(lorentzVector);
				else if (isCaloMuonId && m_dataYear == 2012) muonEff = m_caloMuCSF->scaleFactor(charge, lorentzVector);
			}
			m_muonCaloEff.push_back(muonEff);
		}
		else if (!m_isMC)
		{
			Double_t scaleFactor = 1;
			pT = pT * fabs(scaleFactor);
			p = p * fabs(scaleFactor);
			E = E * fabs(scaleFactor);
		}
		currMuon.pt() = pT;
		currMuon.E() = E;

		// Muon Uncertainty Correction
		// muon type, =1 for combined muons, =2 for calorimeter and segment tagged muons, =3 for stand-alone muons 
		Int_t type = 2;
		TLorentzVector muLorentz;
		phi = currMuon.phi();
		E = currMuon.E();
		eta = currMuon.eta();
		pT = currMuon.pt();

	}

}

void MuonCaloSmear::initializeMuonObj()
{
	m_muon = &(m_event->mu_calo);

	Double_t pT;
	Double_t phi;
	Double_t eta;
	Double_t pTID;
	Double_t phiID;
	Double_t etaID;
	for (Int_t i = 0; i < m_muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*m_muon)[i];

		currMuon.me_pt = currMuon.pt();
		currMuon.me_phi() = currMuon.phi();
		currMuon.me_eta = currMuon.eta();

		currMuon.id_pt = sin(currMuon.id_theta()) / fabs(currMuon.id_qoverp());
		currMuon.id_phi() = currMuon.id_phi();
		currMuon.id_eta = -TMath::Log(tan(currMuon.id_theta()/2));

		currMuon.id_pt_unsmeared = currMuon.id_pt;
		currMuon.me_pt_unsmeared = currMuon.me_pt;
		currMuon.cb_pt_unsmeared = currMuon.pt();
	}
}