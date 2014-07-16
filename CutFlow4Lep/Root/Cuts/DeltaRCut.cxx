#include "CutFlow4Lep/Cuts/DeltaRCut.h"

DeltaRCut::DeltaRCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: QuadLeptonCuts(tEvent, tQuadLepton)
{

}

DeltaRCut::~DeltaRCut()
{

}

Bool_t DeltaRCut::passedCut()
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