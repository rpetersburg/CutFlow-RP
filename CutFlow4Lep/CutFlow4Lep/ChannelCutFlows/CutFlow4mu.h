#ifndef CUTFLOW4MU_H
#define CUTFLOW4MU_H

#include "CutFlow4Lep/ChannelCutFlows/ChannelCutFlow.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

using namespace std;
class CutFlow4mu : public ChannelCutFlow
{
	public:
		CutFlow4mu(vector<Muon*> *tInitMuonVec);
		~CutFlow4mu();

		Bool_t passedCut();

	protected:

	private:
		vector<Muon*> *m_initMuonVec;
};
#endif