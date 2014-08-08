#ifndef ZMASSCONSTRAINTCORRECTION
#define ZMASSCONSTRAINTCORRECTION

#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"
#include "CutFlow4Lep/Corrections/Correction.h"

#include "ZMassConstraint/ZMassConstraint.h"

#include "egammaFourMomentumError/GeneralUtils.h" // for InvariantMass

using namespace std;
class ZMassConstraintCorrection : public Correction
{
public:
	ZMassConstraintCorrection(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~ZMassConstraintCorrection();

	void executeCorrection();
	void setMuonType(Int_t tMuonType) {m_muonType = tMuonType;};
	void setQuadLepton(QuadLepton *tQuadLepton) { m_quadLepton = tQuadLepton; };

protected:

private:
	void initializeCorrectionTool();

	void correctZBoson(DiLepton *diLepton, vector<TLorentzVector> zMomentum);

	void fillInputWithZBoson(ZMassConstraint::ConstraintFitInput input, DiLepton *diLepton);

	QuadLepton *m_quadLepton;
	Int_t m_muonType;
	ZMassConstraint::ConstraintFit *m_constraintFitTool;

	vector<TLorentzVector> m_lepMomentum;
	vector<TMatrixD> m_lepError;
}

#endif