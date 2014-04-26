#ifndef MUONOVERLAP_H
#define MUONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

#include "D3PDReader/MuonD3PDObject.h"

#include <TMath.h>

class MuonOverlap : public Overlap<Muon>
{
	public:
		MuonOverlap(vector<Muon*> *tInitMuonVec);
		~MuonOverlap();

		void removeOverlap();

	protected:

	private:
		Bool_t isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t isGoodStandAloneMuon(D3PDReader::MuonD3PDObjectElement *currMuon);
};
#endif