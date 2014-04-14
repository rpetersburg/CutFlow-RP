#ifndef PHOTONSMEAR_H
#define PHOTONSMEAR_H

#include "CutFlow4Lep/Smear/EGammaSmear.h"
#include "CutFlow4Lep/StructDef.h"

#include "ElectronPhotonFourMomentumCorrection/egammaEnergyCorrectionTool.h"

using namespace std;
class PhotonSmear : public EGammaSmear
{
	public:
		PhotonSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tCurrDataCalibration, Int_t tRunNumber_sf);
		~PhotonSmear();

		void executeSmear();
	protected:

	private:
};
#endif