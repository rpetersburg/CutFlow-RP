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
		void setQuadLepton(QuadLepton *tQuadLepton) {m_quadLepton = tQuadLepton;};

	protected:

	private:
		QuadLepton *m_quadLepton;
		Bool_t m_doZ4lAnalysis;
}

#endif