#ifndef D0SIGCUT_H
#define D0SIGCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

using namespace std;
class D0SigCut : public QuadLeptonCuts
{
public:
	D0SigCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~D0SigCut();

	Bool_t passedCut();

protected:

private:
};
#endif