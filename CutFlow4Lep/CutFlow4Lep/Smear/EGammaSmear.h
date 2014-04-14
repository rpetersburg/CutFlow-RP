#ifndef EGAMMASMEAR_H
#define EGAMMASMEAR_H

#include "CutFlow4Lep/Smear/Smear.h"
#include "CutFlow4Lep/StructDef.h"

#include "ElectronPhotonFourMomentumCorrection/egammaEnergyCorrectionTool.h"

using namespace std;
class EGammaSmear : public Smear
{
	public:
		EGammaSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tCurrDataCalibration, Int_t tRunNumber_sf);
		~EGammaSmear();

		virtual void executeSmear() = 0;
	protected:
		void initializeSmearObj();
		
		AtlasRoot::egammaEnergyCorrectionTool *m_smearTool;

		Int_t m_currMCCollection;
		Int_t m_currDataCalibration;
		Int_t m_runNumber_sf;

	private:

};
#endif