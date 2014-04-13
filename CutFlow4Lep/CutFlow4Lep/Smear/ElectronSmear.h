#ifndef ELECTRONSMEAR_H
#define ELECTRONSMEAR_H

#include "CutFlow4Lep/Smear/Smear.h"
#include "CutFlow4Lep/StructDef.h"

#include "PATCore/PATCoreEnums.h"

#include "ElectronPhotonFourMomentumCorrection/egammaEnergyCorrectionTool.h"
#include "ElectronEfficiencyCorrection/TElectronEfficiencyCorrectionTool.h"
#include "egammaFourMomentumError/egammaFourMomentumError.h"

using namespace std;
class ElectronSmear : public Smear
{
	public:
		ElectronSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tCurrDataCalibration, Int_t tRunNumber_sf);
		~ElectronSmear();

		void executeSmear();
		vector<Double_t> getEff() {return m_electronEff;};
		vector<Double_t> getMomentumError() {return m_momentumErrorVec;};
		vector<float> getbfEP_cl_ET() {return m_bfEP_cl_ET;};

	protected:

	private:
		void initializeSmearObj();
		void initializeEfficiencyTool();
		void initializeMomentumErrorTool();

		AtlasRoot::egammaEnergyCorrectionTool *m_smearTool;
		Root::TElectronEfficiencyCorrectionTool *m_idEffTool;
		Root::TElectronEfficiencyCorrectionTool *m_recoEffTool;
		egammaFourMomentumError *m_momentumErrorTool;

		Int_t m_currMCCollection;
		Int_t m_currDataCalibration;
		Int_t m_runNumber_sf;

		vector<Double_t> m_electronEff;
		vector<Double_t> m_momentumErrorVec;
		vector<float> m_bfEP_cl_pT;
		vector<float> m_bfEP_cl_ET;
};
#endif