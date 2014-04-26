#include "CutFlow4Lep/Overlap/MuonElectronOverlap.h"

MuonElectronOverlap::MuonElectronOverlap(vector<Muon*> *tInitMuonVec, vector<Electron*> *tElectronVec)
	: Overlap(tInitMuonVec), m_electronVec(tElectronVec)
{
	m_dataYear = (*m_initParticleVec)[0]->getDataYear();
}

MuonElectronOverlap::~MuonElectronOverlap()
{

}

void MuonElectronOverlap::removeOverlap()
{
		vector<Muon*>::iterator currMuonObj = m_initParticleVec->begin();
	for ( ; currMuonObj != m_initParticleVec->end(); ++currMuonObj)
	{
		if (!overlapsElectron(*currMuonObj))
			m_goodParticleVec.push_back(*currMuonObj);
	}
}

Bool_t MuonElectronOverlap::overlapsElectron(Muon *currMuonObj)
{
	D3PDReader::MuonD3PDObjectElement *currMuon = currMuonObj->getMuon();

	Double_t muonEta = -log(tan(currMuon->id_theta() * 0.5));
	Double_t muonPhi = currMuon->id_phi();

	vector<Electron*>::iterator currElectronObj = m_electronVec->begin();
	for ( ; currElectronObj != m_electronVec->end(); ++currElectronObj)
	{
		D3PDReader::ElectronD3PDObjectElement *currElectron = (*currElectronObj)->getElectron();

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

		if (deltaR(muonEta, muonPhi, electronEta, electronPhi) < 0.02) 
			return true;
	}
	return false;
}