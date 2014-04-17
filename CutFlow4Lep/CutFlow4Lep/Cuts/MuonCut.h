#ifndef MUONCUT_H
#define MUONCUT_H

#include "CutFlow4Lep/Cuts/Cuts.h"
#include "CutFlow4Lep/Overlap/MuonOverlap.h"

class MuonCut : public Cuts
{
	public:
		MuonCut(D3PDReader::Event *tEvent);
		~MuonCut();

		Bool_t passedCut();
		

	protected:

	private:
		void initializeMuonObj();

		Bool_t passedStacoCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedCaloCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedStandAloneCut(D3PDReader::MuonD3PDObjectElement *currMuon);

		D3PDReader::MuonD3PDObject *m_muonCalo;
		D3PDReader::MuonD3PDObject *m_muonStaco;

		Double_t m_z0Cut;
		Double_t m_d0Cut;
}

#endif