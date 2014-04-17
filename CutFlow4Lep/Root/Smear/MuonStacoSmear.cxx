#include "CutFlow4Lep/Smear/MuonStacoSmear.h"

MuonStacoSmear::MuonStacoSmear(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting)
	: MuonSmear4Lep(tEvent, tPileupReweighting)
{
	initializeMuonObj();
}

MuonStacoSmear::~MuonStacoSmear()
{

}

void MuonStacoSmear::executeSmear()
{
	D3PDReader::MuonD3PDObjectElement *currMuon;
	for (Int_t i = 0; i < m_muon->n(); i++)
	{
		currMuon = &(*m_muon)[i];

		Double_t phi = currMuon->phi();
		Double_t E = currMuon->E();
		Double_t charge = currMuon->charge();
		Double_t eta = currMuon->eta();
		Double_t pT = currMuon->pt();
		Double_t p = 0;
		Double_t ptMs = sin(currMuon->me_theta()) / fabs(double(currMuon->me_qoverp()));

		Double_t pTID;
		if (currMuon->isStandAloneMuon()) pTID = pT;
		else pTID = sin(currMuon->id_theta()) / fabs(double(currMuon->id_qoverp()));

		// Actual smearing occurs here
		Double_t smear = 1;
		if (m_isMC)
		{
			m_smearTool->SetSeed(m_eventNumber, i);
			
			if (currMuon->isCombinedMuon())
			{
				Double_t pTCBSmeared = pT;
				Double_t pTMSSmeared = pT;
				Double_t pTIDSmeared = pT;

				if (fabs(eta) <= 2.7)
				{
					m_smearTool->Event(ptMs, pTID, pT, eta, charge, phi);
					pTCBSmeared = m_smearTool->pTCB();
					pTMSSmeared = m_smearTool->pTMS();
					pTIDSmeared = m_smearTool->pTID();

					currMuon->me_pt = pTMSSmeared;
					currMuon->id_pt = pTIDSmeared;
				}

				smear = pTCBSmeared / pT;
				pT = pTCBSmeared;
				E = E * smear;
			}
			else if (currMuon->isStandAloneMuon())
			{
				Double_t pTMSSmeared = pT;
				if (fabs(eta) <= 2.7)
				{
					m_smearTool->Event(pT, eta, "MS", charge, phi);
					pTMSSmeared = m_smearTool->pTMS();
				}
				smear = pTMSSmeared / pT;
				pT = pTMSSmeared;
				E = E * smear;

				currMuon->id_pt = pTMSSmeared;
				currMuon->me_pt = pTMSSmeared;
			}
			else if (currMuon->isSegmentTaggedMuon())
			{
				Double_t pTIDSmeared = pT;
				if (fabs(eta) <= 2.7)
				{
					m_smearTool->Event(pT, eta, "ID", charge, phi);
					pTIDSmeared = m_smearTool->pTID();
				}
				smear = pTIDSmeared / pT;
				pT = pTIDSmeared;
				E = E * smear;

				currMuon->id_pt = pTIDSmeared;
				currMuon->me_pt = pTIDSmeared;
			}
			else {cout << "MuonStacoSmear::executeSmear(): Unrecognized muon type" << endl;}

		}

		// Muon Efficiency
		if (m_isMC)
		{
			Double_t muEff = 1;
			Double_t pTCut = 5 * 1000;

			if (pT >= pTCut)
			{
				TLorentzVector lorentzVector;
				lorentzVector.SetPtEtaPhiE(pT, eta, phi, E);

				if (!currMuon->isStandAloneMuon())
					muEff = m_stacoCSF->scaleFactor(charge, lorentzVector);
				else if (fabs(eta) >= 2.5 && fabs(eta) <= 2.7)
				{
					if (m_dataYear == 2011) muEff = m_stacoSACSF->scaleFactor(lorentzVector);
					else if (m_dataYear == 2012) muEff = m_stacoSACSF->scaleFactor(charge, lorentzVector);
				}
			}
			m_muonEff.push_back(muEff);
		}

		// Scaling Data
		else if (!m_isMC)
		{
			Double_t scaleFactor = 1;
			pT = pT * fabs(scaleFactor);
			p = p * fabs(scaleFactor);
			E = E * fabs(scaleFactor);
		}
		currMuon->pt() = pT;
		currMuon->E() = E;

		m_smear.push_back(smear);

		// Warning for non-physical smear values
		if (pT < 1e-3 || pT > 1e7) cout << "MuonStacoSmear::executeSmear(): Muon Smear values are not physical" << endl;

		// Correct for uncertainties (found in muon parent class)
		muonUncertaintyCorrection(currMuon);
	}
}

void MuonStacoSmear::initializeMuonObj()
{
	m_muon = &(m_event->mu_staco);

	Double_t pT;
	Double_t phi;
	Double_t eta;
	Double_t pTID;
	Double_t phiID;
	Double_t etaID;
	D3PDReader::MuonD3PDObjectElement *currMuon;
	for (Int_t i = 0; i < m_muon->n(); i++)
	{
		currMuon = &(*m_muon)[i];

		if (currMuon->isStandAloneMuon())
		{
			pTID = currMuon->pt();
			phiID = currMuon->phi();
			etaID = currMuon->eta();
		}
		else
		{
			pTID = sin(currMuon->me_theta()) / fabs(currMuon->id_qoverp());
			phiID = currMuon->id_phi();
			etaID = -TMath::Log(tan(currMuon->id_theta()/2));
		}

		if (currMuon->isSegmentTaggedMuon())
		{
			pT = currMuon->pt();
			phi = currMuon->phi();
			eta = currMuon->eta();
		}
		else
		{
			pT = sin(currMuon->me_theta()) / fabs(currMuon->me_qoverp());
			phi = currMuon->me_phi();
			eta = -TMath::Log(tan(currMuon->me_theta()/2));
		}

		currMuon->me_pt = pT;
		currMuon->me_phi() = phi;
		currMuon->me_eta = eta;

		currMuon->id_pt = pTID;
		currMuon->id_phi() = phiID;
		currMuon->id_eta = etaID;

		currMuon->id_pt_unsmeared = currMuon->id_pt;
		currMuon->me_pt_unsmeared = currMuon->me_pt;
		currMuon->cb_pt_unsmeared = currMuon->pt();
	}
}