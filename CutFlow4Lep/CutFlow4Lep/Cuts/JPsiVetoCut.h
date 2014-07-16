#ifndef JPSIVETOCUT_H
#define JPSIVETOCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

using namespace std;
class JPsiVetoCut : public QuadLeptonCuts
{
public:
	JPsiVetoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~JPsiVetoCut();

	Bool_t passedCut();

protected:

private:
};

#endif