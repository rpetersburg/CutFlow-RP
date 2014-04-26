#include "CutFlow4Lep/Overlap/JetsElectronOverlap.h"

JetsElectronOverlap::JetsElectronOverlap(vector<Jets*> *tInitJetsVec, vector<Electron*> *tElectronVec)
	: Overlap(tInitJetsVec), m_electronVec(tElectronVec)
{
	m_dataYear = (*m_initParticleVec)[0]->getDataYear();
}

JetsElectronOverlap::~JetsElectronOverlap()
{

}

void JetsElectronOverlap::removeOverlap()
{
	vector<Jets*>::iterator currJetsObj = m_initParticleVec->begin();
	for ( ; currJetsObj != m_initParticleVec->end(); ++currJetsObj)
	{
		if (!overlapsElectron(*currJetsObj))
			m_goodParticleVec.push_back(*currJetsObj);
	}
}

Bool_t JetsElectronOverlap::overlapsElectron(Jets *currJetsObj)
{
	D3PDReader::JetD3PDObjectElement *currJets = currJetsObj->getJets();

	Double_t jetsEta = currJets->emscale_eta();
	Double_t jetsPhi = currJets->emscale_phi();

	vector<Electron*>::iterator currElectronObj = m_electronVec->begin();
	for ( ; currElectronObj != m_electronVec->end(); ++currElectronObj)
	{
		D3PDReader::ElectronD3PDObjectElement *currElectron = (*currElectronObj)->getElectron();
		
		Double_t electronEta = currElectron->tracketa();
		Double_t electronPhi = currElectron->trackphi();

		if (deltaR(jetsEta, jetsPhi, electronEta, electronPhi) < 0.2) 
			return true;
	}
	return false;
}