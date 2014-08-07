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
	void initializeCorrectionTool() {};

	void fillCandidateVec(vector<FsrPhotons::FsrCandidate> candidateVec);
	void fill_ph_EtCone40_corr(vector<float> *ph_EtCone40_corr);
	void fill_el_bfEP_cl_pt(vector<float> *el_bfEP_cl_pt);
	
	Bool_t passedFSRCollinear(vector<FsrPhotons::FsrCandidate> candidateVec);
	Bool_t passedFSRFar(vector<FsrPhotons::FsrCandidate> candidateVec);
	PATCore::ParticleType::Type particleType(FsrPhotons::FsrCandidate candidate);
	Double_t zMassWithFSR(DiLepton *diLepton);

	QuadLepton *m_quadLepton;
	vector<ChargedLepton*> m_leptonVec;

	ElectronSmear *m_electronSmearObj;
};
#endif