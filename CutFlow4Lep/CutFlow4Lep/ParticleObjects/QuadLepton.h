#ifndef QUADLEPTON_H
#define QUADLEPTON_H

#include <TLorentzVector.h>
#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/ParticleObjects/DiLepton.h"

using namespace std;
class QuadLepton : public ParticleObject
{
	public:
		QuadLepton(DiLepton *tZ1, DiLepton *tZ2);
		~QuadLepton();

		void setZBosons(DiLepton *tZ1, DiLepton *tZ2);
		DiLepton* getZ1() {return m_z1;};
		DiLepton* getZ2() {return m_z2;};
		vector<ChargedLepton*> getLeptons() {return m_leptonVec;};

		Int_t getType() {return m_type;};

	protected:

	private:
		Int_t m_type;

		DiLepton *m_z1;
		DiLepton *m_z2;

		vector<ChargedLepton*> m_leptonVec;
		vector<TLorentzVector*> m_leptonLorentzVec;
		vector<TLorentzVector*> m_leptonLorentzMEVec;
		vector<TLorentzVector*> m_leptonLorentzIDVec;

};
#endif
