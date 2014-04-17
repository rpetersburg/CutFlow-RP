#include "CutFlow4Lep/Overlap/MuonOverlap.h"

MuonOverlap::MuonOverlap(vector<D3PDReader::MuonD3PDObjectElement*> tInitMuonVec)
	: m_initMuonVec(tInitMuonVec)
{

}

MuonOverlap::~MuonOverlap()
{

}

void MuonOverlap::setInitMuonVec(vector<D3PDReader::MuonD3PDObjectElement*> tInitMuonVec)
{
	m_initMuonVec = tInitMuonVec;
}

void MuonOverlap::removeOverlap()
{
	D3PDReader::MuonD3PDObjectElement *currMuon;

	vector<D3PDReader::MuonD3PDObjectElement*>::iterator itr = m_initMuonVec.begin();
	for ( ; itr != m_initMuonVec.end(); ++itr)
	{
		currMuon = *itr;

		if (currMuon->isCaloMuonId())
		{
			if (isGoodCaloMuon(currMuon))
				m_goodMuonVec.push_back(currMuon);
		}
		else if (currMuon->isStandAloneMuon())
		{
			if (isGoodStandAloneMuon(currMuon))
				m_goodMuonVec.push_back(currMuon);
		}
		else
			m_goodMuonVec.push_back(currMuon);
	}
}

Bool_t MuonOverlap::isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	D3PDReader::MuonD3PDObjectElement *testMuon;

	vector<D3PDReader::MuonD3PDObjectElement*>::iterator itr = m_initMuonVec.begin();
	for ( ; itr != m_initMuonVec.end(); ++itr)
	{
		testMuon = *itr;
		if (currMuon == testMuon) continue;
		else if (testMuon->isCaloMuonId())
		{
			Double_t currEta = -log(tan(currMuon->id_theta()*0.5));
			Double_t currPhi = currMuon->id_phi();

			Double_t testEta = -log(tan(testMuon->id_theta()*0.5));
			Double_t testPhi = testMuon->id_phi();

			if (deltaR(currEta, currPhi, testEta, testPhi) < 0.1)
				return false; // Muon is overlapping with another
		}
	}
	return true; // None of the muons overlapped
}

Bool_t MuonOverlap::isGoodStandAloneMuon(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	D3PDReader::MuonD3PDObjectElement *testMuon;

	vector<D3PDReader::MuonD3PDObjectElement*>::iterator itr = m_initMuonVec.begin();
	for ( ; itr != m_initMuonVec.end(); ++itr)
	{
		testMuon = *itr;
		if (currMuon == testMuon) continue;
		else if (!testMuon->isCaloMuonId() && testMuon->isSegmentTaggedMuon())
		{
			if (deltaR(currMuon->eta(), currMuon->phi(), testMuon->eta(), testMuon->phi()))
				return false; // Muon is overlapping with another
		}
	}
	return true; // None of the muons overlapped
}

Double_t MuonOverlap::deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2)
{
	Double_t deltaEta = (eta1 - eta2);
	Double_t deltaPhi = (fabs(phi1 - phi2) > TMath::Pi()) ? 2*TMath::Pi() - fabs(phi1-phi2) : fabs(phi1 - phi2);

	return sqrt(deltaEta * deltaEta + deltaPhi * deltaPhi);
}