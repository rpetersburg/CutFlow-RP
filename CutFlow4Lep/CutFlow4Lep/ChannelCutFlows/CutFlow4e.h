#ifndef CUTFLOW4E_H
#define CUTFLOW4E_H

#include "CutFlow4Lep/ChannelCutFlows/ChannelCutFlow.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

using namespace std;
class CutFlow4e : public ChannelCutFlow
{
	public:
		CutFlow4e(vector<Electron*> *electronVec);
		~CutFlow4e();

		Bool_t passedCut();

	protected:

	private:
};
#endif