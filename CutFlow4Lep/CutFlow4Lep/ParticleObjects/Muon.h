#ifndef MUON_H
#define MUON_H

#include "D3PDReader/MuonD3PDObject.h"

#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/StructDef.h"

#include <TLorentzVector.h>

using namespace std;
class Muon : public ChargedLepton
{
	public:
		Muon(D3PDReader::MuonD3PDObjectElement *tMuon, Double_t tMuonEff, Double_t tMuonSmear);
		~Muon();

		D3PDReader::MuonD3PDObjectElement* getMuon() {return m_muon;};

	protected:

	private:
		D3PDReader::MuonD3PDObjectElement *m_muon;
};

#endif