#ifndef MUONCUT_H
#define MUONCUT_H

#include "CutFlow4Lep/Cuts/ParticleCuts.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

class MuonCut : public ParticleCuts<Muon>
{
	public:
		MuonCut(D3PDReader::Event *tEvent, vector<Muon*> *tMuonVec);
		~MuonCut();

		Bool_t passedCut();
		void executeCut();

	protected:

	private:
		Bool_t passedStacoCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedCaloCut(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t passedStandAloneCut(D3PDReader::MuonD3PDObjectElement *currMuon);

		Double_t m_z0Cut;
		Double_t m_d0Cut;
};
#endif