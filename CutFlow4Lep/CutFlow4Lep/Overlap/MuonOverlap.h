#ifndef MUONOVERLAP_H
#define MUONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

#include "D3PDReader/MuonD3PDObject.h"

#include <TMath.h>

class MuonOverlap : public Overlap
{
	public:
		MuonOverlap(vector<Muon*> *tInitMuonVec);
		~MuonOverlap();

		void removeOverlap();
		void setInitMuonVec(vector<Muon*> *tInitMuonVec) {m_initMuonVec = tInitMuonVec;};
		vector<Muon*> getGoodMuonVec() {return m_goodMuonVec;};

	protected:

	private:
		Bool_t isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t isGoodStandAloneMuon(D3PDReader::MuonD3PDObjectElement *currMuon);

		Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);

		vector<Muon*> *m_initMuonVec;
		vector<Muon*> m_goodMuonVec;
};
#endif