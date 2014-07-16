#include "CutFlow4Lep/Cuts/D0SigCut.h"

D0SigCut::D0SigCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: QuadLeptonCuts(tEvent, tQuadLepton)
{

}

D0SigCut::~D0SigCut()
{

}

Bool_t D0SigCut::passedCut()
{
	Bool_t passedCut = true;

	Double_t muD0SigCut = 3.5;
	Double_t elD0SigCut = 6.5;

	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();

	vector<ChargedLepton*>::iterator leptonItr = leptonVec.begin();
	for ( ; leptonItr != leptonVec.end(); ++leptonItr)
	{
		ChargedLepton *lepton = *leptonItr;

		Double_t dataD0Sig = 0;
		Bool_t cutLepton = true;

		if (lepton->getFlavor() == Flavor::Electron)
		{
			dataD0Sig = fabs(lepton->getElectron()->trackd0pvunbiased()) / lepton->getElectron()->tracksigd0pvunbiased();

			if (dataD0Sig > elD0SigCut) cutLepton = false;
		}
		else if (lepton->getFlavor() == Flavor::Muon)
		{
			dataD0Sig = fabs(lepton->getMuon()->trackd0pvunbiased()) / lepton->getMuon()->tracksigd0pvunbiased();

			if (dataD0Sig > muD0SigCut) cutLepton = false;
		}
		m_quadLepton->d0SigValPushBack(dataD0Sig);
		m_quadLepton->d0SigPushBack(cutLepton);

		if (!cutLepton) passedCut = false; // If any leptons are cut, function returns false
	}

	return passedCut;
}