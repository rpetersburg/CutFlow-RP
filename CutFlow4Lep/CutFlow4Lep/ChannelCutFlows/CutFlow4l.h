#ifndef CUTFLOW4L_H
#define CUTFLOW4L_H

#include "CutFlow4Lep/ChannelCutFlows/ChannelCutFlow.h"

using namespace std;
class CutFlow4l : public ChannelCutFlow
{
	public:
		CutFlow4l();
		~CutFlow4l();

		Bool_t passedCut();

	protected:

	private:
};
#endif