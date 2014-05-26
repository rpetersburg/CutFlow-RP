#ifndef PARTICLEOBJECT_H
#define PARTICLEOBJECT_H

#include <TLorentzVector.h>

#include "CutFlow4Lep/StructDef.h"

using namespace std;
class ParticleObject
{
	public:
		ParticleObject() {};
		~ParticleObject() {};

		Int_t getType() {return m_type;};

		TLorentzVector* getMomentumVec() {return m_momentum;};
		TLorentzVector* getMomentumMainVec() {return m_momentumMain;};

	protected:
		Int_t m_type;

		TLorentzVector *m_momentum;
		TLorentzVector *m_momentumMain;

	private:
}
#endif