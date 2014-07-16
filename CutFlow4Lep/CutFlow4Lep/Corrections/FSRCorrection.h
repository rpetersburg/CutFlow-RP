#ifndef FSRCORRECTION_H
#define FSRCORRECTION_H

#include "CutFlow4Lep/Corrections/Correction.h"
#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"
#include "CutFlow4Lep/Smear/ElectronSmear.h"

#include "egammaAnalysisUtils/FsrPhotons.h"
#include "egammaAnalysisUtils/CaloIsoCorrection.h"

using namespace std;
class FSRCorrection : public Correction
{
public:
	FSRCorrection(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, ElectronSmear *tElectronSmearObj);
	~FSRCorrection();

	void executeCorrection();

protected:

private:
	QuadLepton *m_quadLepton;

	ElectronSmear *m_electronSmearObj;
};
#endif