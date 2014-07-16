#include "CutFlow4Lep/ParticleObjects/DiLepton.h"

DiLepton::DiLepton(ChargedLepton *tLepton1, ChargedLepton *tLepton2)
{
	setLeptons(tLepton1, tLepton2);
}

DiLepton::~DiLepton()
{

}

void DiLepton::setLeptons(ChargedLepton *tLepton1, ChargedLepton *tLepton2)
{
	if (tLepton1->getCharge() >= 0)
	{
		m_posLepton = tLepton1;
		m_negLepton = tLepton2;
	}
	else
	{
		m_posLepton = tLepton2;
		m_negLepton = tLepton1;
	}

	m_momentum = new TLorentzVector();
	*m_momentum = *m_posLepton->getMomentumVec() + *m_negLepton->getMomentumVec();
	m_momentumMain = new TLorentzVector();
	*m_momentumMain = *m_posLepton->getMomentumMainVec() + *m_negLepton->getMomentumMainVec();
}

Bool_t DiLepton::isEqual(DiLepton *testDiLepton)
{
	if (getPosLepton() == testDiLepton->getPosLepton() ||
			getNegLepton() == testDiLepton->getNegLepton() ||
			getPosLepton() == testDiLepton->getNegLepton() ||
			getNegLepton() == testDiLepton->getPosLepton() )
		return true;
	return false;
}

void DiLepton::setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale)
{
	m_elRescale = telRescale;
	m_posLepton->setElRescale(m_elRescale);
	m_negLepton->setElRescale(m_elRescale);
}