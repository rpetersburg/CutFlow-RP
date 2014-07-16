#ifndef QUADLEPTONCUTS_H
#define QUADLEPTONCUTS_H

#include "CutFlow4Lep/Cuts/Cuts.h"
#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

using namespace std;
class QuadLeptonCuts : public Cuts
{
public:
	QuadLeptonCuts(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton) 
		: Cuts(tEvent), m_quadLepton(tQuadLepton) {};
	~QuadLeptonCuts() {};

	virtual Bool_t passedCut() = 0;

	void setQuadLepton(QuadLepton *tQuadLepton) {m_quadLepton = tQuadLepton;};

protected:
	QuadLepton *m_quadLepton;

private:
};
#endif