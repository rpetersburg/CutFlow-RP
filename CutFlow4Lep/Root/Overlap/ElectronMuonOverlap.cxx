#include "CutFlow4Lep/Overlap/ElectronMuonOverlap.h"

ElectronMuonOverlap::ElectronMuonOverlap(vector<Electron*> *tInitElectronVec, vector<Muon*> *tMuonCaloVec, vector<Muon*> *tMuonStacoVec)
	: Overlap(tInitElectronVec), m_muonCaloVec(tMuonCaloVec), m_muonStacoVec(tMuonStacoVec)
{
	m_dataYear = (*m_initParticleVec)[0]->getDataYear();
}

ElectronMuonOverlap::~ElectronMuonOverlap()
{

}

void ElectronMuonOverlap::removeOverlap()
{
	vector<Electron*>::iterator currElectronObj = m_initParticleVec->begin();
	for ( ; currElectronObj != m_initParticleVec->end(); ++currElectronObj)
	{
		if (!overlapsMuon(*currElectronObj))
			m_goodParticleVec.push_back(*currElectronObj);
	}
}

Bool_t ElectronMuonOverlap::overlapsMuon(Electron *currElectronObj)
{
	D3PDReader::ElectronD3PDObjectElement *currElectron = currElectronObj->getElectron();
	Double_t electronEta, electronPhi;

	if (m_dataYear == 2011)
	{
		electronEta = currElectron->tracketa();
		electronPhi = currElectron->trackphi();
	}
	else if (m_dataYear == 2012)
	{
		electronEta = currElectron->Unrefittedtrack_eta();
		electronPhi = currElectron->Unrefittedtrack_phi();
	}

	vector<Muon*>::iterator currMuonObj = m_muonStacoVec->begin();
	for ( ; currMuonObj != m_muonStacoVec->end(); ++currMuonObj)
	{
		D3PDReader::MuonD3PDObjectElement *currMuon = (*currMuonObj)->getMuon();
		Double_t muonEta = -log(tan(currMuon->id_theta() * 0.5));
		Double_t muonPhi = currMuon->id_phi();

		if (deltaR(muonEta, muonPhi, electronEta, electronPhi) < 0.02)
			return true;
	}

	currMuonObj = m_muonCaloVec->begin();
	for ( ; currMuonObj != m_muonStacoVec->end; ++currMuonObj)
	{
		D3PDReader::MuonD3PDObjectElement *currMuon = (*currMuonObj)->getMuon();
		Double_t muonEta = -log(tan(currMuon->id_theta() * 0.5));
		Double_t muonPhi = currMuon->id_phi();

		if (deltaR(muonEta, muonPhi, electronEta, electronPhi) < 0.02)
			return true;
	}
	return false;
}