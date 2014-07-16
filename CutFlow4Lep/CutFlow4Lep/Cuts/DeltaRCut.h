#ifndef DELTARCUT_H
#define DELTARCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

using namespace std;
class DeltaRCut : public QuadLeptonCuts
{
public:
	DeltaRCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~DeltaRCut();

	Bool_t passedCut();

protected:

private:
};
#endif