#ifndef JETCALIBRATION_H
#define JETCALIBRATION_H

#include "CutFlow4Lep/Corrections/Correction.h"
#include "CutFlow4Lep/StructDef.h"

#include "ApplyJetCalibration/ApplyJetCalibration.h"

using namespace std;
class JetCalibration : public Correction
{
	public:
		JetCalibration(D3PDReader::Event *tEvent, Int_t tCurrMCCollection);
		~JetCalibration();

		void executeCorrection();

	protected:

	private:
		void initializeCorrectionTool();

		Int_t m_currMCCollection;

		JetAnalysisCalib::JetCalibrationTool *m_jetCalibrationTool;
}
#endif