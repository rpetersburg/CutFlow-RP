#include "CutFlow4Lep/Overlap/MuonOverlap.h"

MuonOverlap::MuonOverlap(vector<Muon*> *tInitMuonVec)
	: Overlap(tInitMuonVec)
{
	m_dataYear = (*m_initParticleVec)[0]->getDataYear();
}

MuonOverlap::~MuonOverlap()
{

}

void MuonOverlap::removeOverlap()
{
	D3PDReader::MuonD3PDObjectElement *currMuon;

	vector<Muon*>::iterator currMuonObj = m_initParticleVec->begin();
	for ( ; currMuonObj != m_initParticleVec->end(); ++currMuonObj)
	{
		currMuon = (*currMuonObj)->getMuon();

		if (( currMuon->isCaloMuonId() && isGoodCaloMuon(currMuon) ) ||
			  ( currMuon->isStandAloneMuon() && isGoodStandAloneMuon(currMuon) ) ||
				( !currMuon->isCaloMuonId() && !currMuon->isStandAloneMuon()) )
			m_goodParticleVec.push_back(*currMuonObj);
	}
}

Bool_t MuonOverlap::isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	D3PDReader::MuonD3PDObjectElement *testMuon;

	vector<Muon*>::iterator currMuonObj = m_initParticleVec->begin();
	for ( ; currMuonObj != m_initParticleVec->end(); ++currMuonObj)
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

	vector<Muon*>::iterator currMuonObj = m_initParticleVec->begin();
	for ( ; currMuonObj != m_initParticleVec->end(); ++currMuonObj)
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