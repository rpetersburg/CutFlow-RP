#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

QuadLepton::QuadLepton(DiLepton *tZ1, DiLepton *tZ2)
{
	setZBosons(tZ1, tZ2);
}

QuadLepton::~QuadLepton()
{

}

void QuadLepton::setZBosons(DiLepton *tZ1, DiLepton *tZ2)
{
	m_z1 = tZ1;
	m_z2 = tZ2;

	if (m_z1->getFlavor() == Flavor::Muon)
	{
		if (m_z1->getFlavor() == Flavor::Muon)
			m_type = QuadType::Mu4;
		else
			m_type = QuadType::Mu2El2;
	}
	else if (m_z1->getFlavor() == Flavor::Electron)
	{
		if (m_z2->getFlavor() == Flavor::Electron)
			m_type = QuadType::El4;
		else
			m_type = QuadType::El2Mu2;
	}

	m_momentum = new TLorentzVector();
	*m_momentum = *m_z1->getMomentumVec() + *m_z2->getMomentumVec();
	m_momentumMain = new TLorentzVector();
	*m_momentumMain = *m_z1->getMomentumMainVec() + *m_z2->getMomentumMainVec();

	m_leptonVec.clear();
	m_leptonLorentzVec.clear();
	m_leptonLorentzMEVec.clear();
	m_leptonLorentzIDVec.clear();

	m_leptonVec.push_back(m_z1->getPosLepton());
	m_leptonVec.push_back(m_z1->getNegLepton());
	m_leptonVec.push_back(m_z2->getPosLepton());
	m_leptonVec.push_back(m_z2->getNegLepton());

	vector<ChargedLepton*>::iterator currLepton = m_leptonVec.begin();
	for ( ; currLepton != m_leptonVec.end(); ++currLepton)
	{
		m_leptonLorentzVec.push_back((*currLepton)->getMomentumVec());
		m_leptonLorentzMEVec.push_back((*currLepton)->getMomentumMEVec());
		m_leptonLorentzIDVec.push_back((*currLepton)->getMomentumIDVec());
	}

}