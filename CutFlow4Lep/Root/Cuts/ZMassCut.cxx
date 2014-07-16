#include "CutFlow4Lep/Cuts/ZMassCut.h"

ZMassCut::ZMassCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, Int_t tDoZ4lAnalysis)
	: QuadLeptonCuts(tEvent, tQuadLepton), m_doZ4lAnalysis(tDoZ4lAnalysis)
{
	
}

ZMassCut::~ZMassCut()
{

}

Bool_t ZMassCut::passedCut()
{
	return (passedZ1MassCut() && passedZ2MassCut());
}

Bool_t ZMassCut::passedZ1MassCut()
{
	Double_t z1Mass = m_quadLepton->getZ1()->getMomentumVec()->M();
	if (m_doZ4lAnalysis && z1Mass <= 20 * 1000) return false;
	else if (z1Mass <= 50 * 1000 || z1Mass >= 106 * 1000) return false;
}

Bool_t ZMassCut::passedZ2MassCut()
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