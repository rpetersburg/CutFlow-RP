#include "CutFlow4Lep/Overlap/MuonOverlap.h"

MuonOverlap::MuonOverlap(vector<Muon*> *tInitMuonVec)
	: m_initMuonVec(tInitMuonVec)
{

}

MuonOverlap::~MuonOverlap()
{

}

void MuonOverlap::removeOverlap()
{
	D3PDReader::MuonD3PDObjectElement *currMuon;

	vector<Muon*>::iterator currMuonObj = m_initMuonVec->begin();
	for ( ; currMuonObj != m_initMuonVec->end(); ++currMuonObj)
	{
		currMuon = (*currMuonObj)->getMuon();

		if (( currMuon->isCaloMuonId() && isGoodCaloMuon(currMuon) ) ||
			  ( currMuon->isStandAloneMuon() && isGoodStandAloneMuon(currMuon) ) ||
				( !currMuon->isCaloMuonId() && !currMuon->isStandAloneMuon()) )
			m_goodMuonVec.push_back(*currMuonObj);
	}
}

Bool_t MuonOverlap::isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	D3PDReader::MuonD3PDObjectElement *testMuon;

	vector<Muon*>::iterator currMuonObj = m_initMuonVec->begin();
	for ( ; currMuonObj != m_initMuonVec->end(); ++currMuonObj)
	{
		testMuon = (*currMuonObj)->getMuon();
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

	vector<Muon*>::iterator currMuonObj = m_initMuonVec->begin();
	for ( ; currMuonObj != m_initMuonVec->end(); ++currMuonObj)
	{
		testMuon = (*currMuonObj)->getMuon();
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