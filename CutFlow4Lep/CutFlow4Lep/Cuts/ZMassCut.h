#ifndef ZMASSCUT_H
#define ZMASSCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

using namespace std;
class ZMassCut : public QuadLeptonCuts
{
public:
	ZMassCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, Int_t tDoZ4lAnalysis);
	~ZMassCut();

	Bool_t passedCut();

protected:

private:
	Bool_t passedZ1MassCut();
	Bool_t passedZ2MassCut();

	Int_t m_doZ4lAnalysis;
};
#endif