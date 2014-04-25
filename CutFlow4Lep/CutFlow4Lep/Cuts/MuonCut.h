#ifndef MUONCUT_H
#define MUONCUT_H

#include "CutFlow4Lep/Cuts/Cuts.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

class MuonCut : public Cuts
{
	public:
		MuonCut(D3PDReader::Event *tEvent, vector<Muon*> *tMuonVec);
		~MuonCut();

		Bool_t passedCut();
		
		vector<Muon*> getCutMuonVec() {return m_cutMuonVec;};

	protected:

	private:
		Bool_t passedStacoCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedCaloCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedStandAloneCut(D3PDReader::MuonD3PDObjectElement *currMuon);

		vector<Muon*> *m_initMuonVec;
		vector<Muon*> m_cutMuonVec;

		Double_t m_z0Cut;
		Double_t m_d0Cut;
}
#endif