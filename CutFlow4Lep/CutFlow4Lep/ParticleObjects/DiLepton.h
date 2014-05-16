#ifndef DILEPTON_H
#define DILEPTON_H

#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"

using namespace std;
class DiLepton : public ParticleObject
{
	public:
		DiLepton(ChargedLepton *tLepton1, ChargedLepton *tLepton2);
		~DiLepton();

		void setLeptons(ChargedLepton *tLepton1, ChargedLepton *tLepton2);
		ChargedLepton* getPosLepton() {return m_posLepton;};
		ChargedLepton* getNegLepton() {return m_negLepton;};

		Bool_t isEqual(DiLepton *testDiLepton);

		Int_t getFlavor() {return m_flavor;};

		TLorentzVector* getMomentumVec() {return m_momentum;};
		TLorentzVector* getMomentumMainVec() {return m_momentumMain;};
	protected:

	private:
		ChargedLepton *m_posLepton;
		ChargedLepton *m_negLepton;

		Int_t m_flavor;

		TLorentzVector *m_momentum;
		TLorentzVector *m_momentumMain;
};
#endif