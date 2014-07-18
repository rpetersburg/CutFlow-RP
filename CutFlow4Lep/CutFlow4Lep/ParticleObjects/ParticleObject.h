#ifndef PARTICLEOBJECT_H
#define PARTICLEOBJECT_H

#include <TLorentzVector.h>

#include "CutFlow4Lep/StructDef.h"
#include "ZMassConstraint/CovMatrixTools.h"

#include "ElectronPhotonFourMomentumCorrection/egammaEnergyCorrectionTool.h"
#include "Matrix/Matrix.h"

using namespace std;
class ParticleObject
{
	public:
		ParticleObject() {};
		~ParticleObject() {};

		Int_t getType() {return m_type;};

		TLorentzVector* getMomentumVec() {return m_momentum;};
		TLorentzVector* getMomentumMainVec() {return m_momentumMain;};

		virtual void setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale);

		AtlasRoot::egammaEnergyCorrectionTool *m_elRescale;

	protected:
		Int_t m_type;

		TLorentzVector *m_momentum;
		TLorentzVector *m_momentumMain;
	private:
};
#endif