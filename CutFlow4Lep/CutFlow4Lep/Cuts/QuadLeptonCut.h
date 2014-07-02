#ifndef QUADLEPTONCUT_H
#define QUADLEPTONCUT_H

#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

using namespace std;
class QuadLeptonCut
{
	public:
		QuadLeptonCut(QuadLepton *tQuadLepton, Bool_t tDoZ4lAnalysis);
		QuadLeptonCut(Bool_t tDoZ4lAnalysis);
		~QuadLeptonCut();

		Bool_t passedCut();
		Bool_t passedZ1MassCut();
		Bool_t passedZ2MassCut();
		Bool_t passedDeltaRCut();
		Bool_t passedJPsiVeto();
		Bool_t passedTrackIsoCut();
		Bool_t passedCaloIsoCut();
		Bool_t passedD0SigCut();
		vector<Double_t> getTrackPT();
		void setQuadLepton(QuadLepton *tQuadLepton) {m_quadLepton = tQuadLepton;};

	protected:

	private:
		QuadLepton *m_quadLepton;
		Bool_t m_doZ4lAnalysis;
}

#endif