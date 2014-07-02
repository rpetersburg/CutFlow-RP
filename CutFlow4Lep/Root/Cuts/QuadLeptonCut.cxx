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
	if (!passedZ1MassCut()) return false;
	if (!passedZ2MassCut()) return false;
	if (!passedDeltaRCut()) return false;
	if (!passedJPsiVeto()) return false;

}

Bool_t QuadLeptonCut::passedZ1MassCut()
{
	Double_t z1Mass = m_quadLepton->getZ1()->getMomentumVec()->M();
	if (m_doZ4lAnalysis && z1Mass <= 20 * 1000) return false;
	else if (z1Mass <= 50 * 1000 || z1Mass >= 106 * 1000) return false;
}

Bool_t QuadLeptonCut::passedZ2MassCut()
{
	Double_t z2Mass = m_quadLepton->getZ2()->getMomentumVec()->M();
	Double_t quadLeptonMass = m_quadLepton->getMomentumVec()->M();

	const Int_t nBin = 2;
	Double_t mass[nBin] = {140.0 * 1000, 190.0 * 1000};
	Double_t cut[nBin] = {12.0 * 1000, 50.0 * 1000};

	if (m_doZ4lAnalysis) // Overwrite for Z->4l Analysis
	{
		mass[0] = -1;
		mass[1] = 999999999;
		cut[0] = 5.0 * 1000;
		cut[1] = 5.0 * 1000;
	}

	Int_t index = -1;
	for (Int_t i = 0; i < nBin; i++)
		if (quadLeptonMass > mass[i]) index = i;

	Double_t z2CutVal = 0;
	if (index == -1) z2CutVal = cut[0];
	else if (index == nBin - 1) z2CutVal = cut[nBin - 1];
	else z2CutVal = cut[index] + (quadLeptonMass - mass[index]) *
									(cut[index+1] - cut[index]) / (mass[index+1] - mass[index]);

	if (z2Mass > z2CutVal && z2Mass < 115 * 1000) return true;
	return false;
}

Bool_t QuadLeptonCut::passedDeltaRCut()
{
	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();

	vector<ChargedLepton*>::iterator iLeptonItr = leptonVec.begin();
	for ( ; iLeptonItr != leptonVec.end(); ++iLeptonItr)
	{
		ChargedLepton *iLepton = *iLeptonItr;

		vector<ChargedLepton*>::iterator jLeptonItr = iLeptonItr + 1;
		for ( ; jLeptonItr != leptonVec.end(); ++jLeptonItr)
		{
			ChargedLepton *jLepton = *jLeptonItr;
			
			// The cut value depends on the flavor of each lepton
			Double_t minDeltaR = 0;
			if (iLepton->getFlavor() == jLepton->getFlavor()) minDeltaR = 0.10;
			else minDeltaR = 0.2;
			// Calculate the data's Delta R value
			Double_t dataDeltaR = iLepton->getMomentumVec()->DeltaR(*(jLepton->getMomentumVec()));
			// Determine if cut is necessary
			if (dataDeltaR < minDeltaR) return false;
		}
	}
	return true;
}

Bool_t QuadLeptonCut::passedJPsiVeto()
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

vector<Double_t> QuadLeptonCut::getTrackPT()
{
	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();
	vector<Double_t> trackPT; // Container
	Double_t minDeltaR = 0.20; // Delta R Cut

	vector<ChargedLepton*>::iterator iLeptonItr;
	vector<ChargedLepton*>::iterator jLeptonItr;
}