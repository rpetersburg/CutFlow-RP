#ifndef QUADLEPTONCUT_H
#define QUADLEPTONCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "CutFlow4Lep/Cuts/ZMassCut.h"
#include "CutFlow4Lep/Cuts/DeltaRCut.h"
#include "CutFlow4Lep/Cuts/JPsiVetoCut.h"
#include "CutFlow4Lep/Cuts/TrackIsoCut.h"
#include "CutFlow4Lep/Cuts/CaloIsoCut.h"
#include "CutFlow4Lep/Cuts/D0SigCut.h"

#include "CutFlow4Lep/Corrections/FSRCorrection.h"

#include "egammaFourMomentumError/GeneralUtils.h"

using namespace std;
class QuadLeptonCut : public QuadLeptonCuts
{
	public:
		QuadLeptonCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, Bool_t tDoZ4lAnalysis);
		~QuadLeptonCut();

		void init();

		Bool_t passedCut();

		void fillEventVarInfo(QuadLepton *quadLepton);

		void setQuadLepton(QuadLepton *tQuadLepton);

	protected:

	private:
		void massCalc();

		QuadLepton *m_quadLepton;
		Bool_t m_doZ4lAnalysis;
		JPsiVetoCut *m_jPsiVetoCut;
		TrackIsoCut *m_trackIsoCut;
		CaloIsoCut *m_caloIsoCut;
		D0SigCut *m_d0SigCut;
		ZMassCut *m_zMassCut;
		DeltaRCut *m_deltaRCut;

		FSRCorrection *m_fsrCorrection;
}

#endif