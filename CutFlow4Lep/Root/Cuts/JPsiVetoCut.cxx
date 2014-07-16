#include "CutFlow4Lep/Cuts/JPsiVetoCut.h"

JPsiVetoCut::JPsiVetoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: QuadLeptonCuts(tEvent, tQuadLepton)
{

}

JPsiVetoCut::~JPsiVetoCut()
{

}

Bool_t JPsiVetoCut::passedCut()
{
	// Only for quadleptons with 4 of the same particles
	if (m_quadLepton->getType() == QuadType::El2Mu2 || m_quadLepton->getType() == QuadType::Mu2El2)
		return true;

	Double_t jPsiVeto = 5 * 1000;

	TLorentzVector crossPair1 = *(m_quadLepton->getZ1()->getPosLepton()->getMomentumVec()) +
															*(m_quadLepton->getZ2()->getNegLepton()->getMomentumVec());
	TLorentzVector crossPair2 = *(m_quadLepton->getZ1()->getNegLepton()->getMomentumVec()) +
															*(m_quadLepton->getZ2()->getPosLepton()->getMomentumVec());

	if (crossPair1.M() < jPsiVeto || crossPair2.M() < jPsiVeto)
		return false;

	return true;
}