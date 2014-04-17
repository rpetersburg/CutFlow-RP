#ifndef MUONOVERLAP_H
#define MUONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"

#include "D3PDReader/MuonD3PDObject.h"

#include <TMath.h>

class MuonOverlap : public Overlap
{
	public:
		MuonOverlap(vector<D3PDReader::MuonD3PDObjectElement*> tInitMuonVec);
		~MuonOverlap();

		void removeOverlap();
		void setInitMuonVec(vector<D3PDReader::MuonD3PDObjectElement*> tInitMuonVec);
		vector<D3PDReader::MuonD3PDObjectElement*> getGoodMuonVec() {return m_goodMuonVec;};


	protected:

	private:
		Bool_t isGoodCaloMuon(D3PDReader::MuonD3PDObjectElement *currMuon);
		Bool_t isGoodStandAloneMuon(D3PDReader::MuonD3PDObjectElement *currMuon);

		Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);

		vector<D3PDReader::MuonD3PDObjectElement*> m_initMuonVec;
		vector<D3PDReader::MuonD3PDObjectElement*> m_goodMuonVec;
};
#endif