#ifndef DILEPTON_H
#define DILEPTON_H

#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"

using namespace std;
class DiLepton : public ParticleObject
{
	public:
		DiLepton(ChargedLepton *tLepton1, ChargedLepton *tLepton2);
		~DiLepton();

		void setLeptons(ChargedLepton *tLepton1, ChargedLepton *tLepton2);
		ChargedLepton* getPosLepton() {return m_posLepton;};
		ChargedLepton* getNegLepton() {return m_negLepton;};

		Bool_t isEqual(DiLepton *testDiLepton);

		Int_t getFlavor() {return m_flavor;};
		Int_t getDataYear() {return m_posLepton->getDataYear();};

		Bool_t getHasFSR() {return m_hasFSR;};
		TLorentzVector getFSRMomentum() {return m_fsrMomentum;};
		TMatrixD getFSRError() {return m_fsrError;};
		CLHEP::HepMatrix getHepFSRError() {return m_hepFSRError;};

		void setHasFSR(Bool_t tHasFSR) {m_hasFSR = tHasFSR;};
		void setFSRMomentum(TLorentzVector tFSRMomentum) {m_fsrMomentum = tFSRMomentum;};
		void setFSRError(TMatrixD tFSRError) {m_fsrError = tFSRError;};
		void setHepFSRError(CLHEP::HepMatrix tHepFSRError) {m_hepFSRError = tHepFSRError;};

		void setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale);

	protected:

	private:
		ChargedLepton *m_posLepton;
		ChargedLepton *m_negLepton;

		Int_t m_flavor;

		Bool_t m_hasFSR;
		TLorentzVector m_fsrMomentum;
		TMatrixD m_fsrError;
		CLHEP::HepMatrix m_hepFSRError;
};
#endif