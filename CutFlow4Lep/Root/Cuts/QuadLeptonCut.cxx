#include "CutFlow4Lep/Cuts/QuadLeptonCut.h"

QuadLeptonCut::QuadLeptonCut(QuadLepton *tQuadLepton, Bool_t tDoZ4lAnalysis)
	: m_quadLepton(tQuadLepton), m_doZ4lAnalysis(tDoZ4lAnalysis)
{

}

QuadLeptonCut::QuadLeptonCut(Bool_t tDoZ4lAnalysis)
	: m_doZ4lAnalysis(tDoZ4lAnalysis)
{

}

QuadLeptonCut::~QuadLeptonCut()
{

}

Bool_t QuadLeptonCut::passedCut()
{
	// Z1 Mass Cuts
	Double_t z1Mass = m_quadLepton->getZ1()->getMomentumVec()->M();
	if (m_doZ4lAnalysis && z1Mass <= 20 * 1000) return false;
	else if (z1Mass <= 50 * 1000 || z1Mass >= 106 * 1000) return false;

	// Z2 Mass Cut
}