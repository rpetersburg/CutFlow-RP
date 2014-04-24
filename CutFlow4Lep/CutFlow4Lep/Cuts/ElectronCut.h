#ifndef ELECTRONCUT_H
#define ELECTRONCUT_H

#include "CutFlow4Lep/Cuts/Cuts.h"
#include "CutFlow4Lep/Overlap/ElectronOverlap.h"

class ElectronCut : public Cuts
{
	public:
		ElectronCut(D3PDReader::Event *tEvent);
		~ElectronCut();

		Bool_t passedCut();
		

	protected:

	private:
		void initializeElectronObj();

		Bool_t passedStacoCut(D3PDReader::ElectronD3PDObjectElement *currElectron);
		Bool_t passedCaloCut(D3PDReader::ElectronD3PDObjectElement *currElectron);
		Bool_t passedStandAloneCut(D3PDReader::ElectronD3PDObjectElement *currElectron);

		D3PDReader::ElectronD3PDObject *m_ElectronCalo;
		D3PDReader::ElectronD3PDObject *m_ElectronStaco;

		Double_t m_z0Cut;
		Double_t m_d0Cut;
}

#endif