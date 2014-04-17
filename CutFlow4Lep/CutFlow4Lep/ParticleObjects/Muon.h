#ifndef MUON_H
#define MUON_H

#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/StructDef.h"

#include <TLorentzVector.h>

using namespace std;
class Muon : public ChargedLepton
{
	public:
		Muon(D3PDReader::Event *tEvent, D3PDReader::MuonD3PDObjectElement *tMuon);
		Muon(D3PDReader::Event *tEvent, D3PDReader::MuonD3PDObjectElement *tMuon, Double_t tMuonEff, Double_t tMuonSmear);
		~Muon();

		D3PDReader::MuonD3PDObjectElement* getMuon() {return m_muon;};

	protected:

	private:
		void init();
		void setMomentumLorentzVectors();
		void setSysEnergy();
		void setptCone20Correction();
		void setTypes();

		D3PDReader::MuonD3PDObjectElement *m_muon;
};

#endif