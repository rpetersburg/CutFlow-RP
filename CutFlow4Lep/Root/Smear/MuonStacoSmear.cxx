#include "CutFlow4Lep/Smear/MuonStacoSmear.h"

MuonStacoSmear::MuonStacoSmear(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting)
	: MuonSmear4Lep(tEvent, tPileupReweighting)
{

}

MuonStacoSmear::~MuonStacoSmear()
{

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
	for (Int_t i = 0; i < m_muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*m_muon)[i];

		if (currMuon.isStandAloneMuon())
		{
			pTID = currMuon.pt();
			phiID = currMuon.phi();
			etaID = currMuon.eta();
		}
		else
		{
			pTID = sin(currMuon.me_theta()) / fabs(currMuon.id_qoverp());
			phiID = currMuon.id_phi();
			etaID = -TMath::Log(tan(currMuon.id_theta()/2));
		}

		if (currMuon.isSegmentTaggedMuon())
		{
			pT = currMuon.pt();
			phi = currMuon.phi();
			eta = currMuon.eta();
		}
		else
		{
			pT = sin(currMuon.me_theta()) / fabs(currMuon.me_qoverp());
			phi = currMuon.me_phi();
			eta = -TMath::Log(tan(currMuon.me_theta()/2));
		}

		currMuon.me_pt = pT;
		currMuon.me_phi() = phi;
		currMuon.me_eta = eta;

		currMuon.id_pt = pTID;
		currMuon.id_phi() = phiID;
		currMuon.id_eta = etaID;

		currMuon.id_pt_unsmeared = currMuon.id_pt;
		currMuon.me_pt_unsmeared = currMuon.me_pt;
		currMuon.cb_pt_unsmeared = currMuon.pt();
	}
}