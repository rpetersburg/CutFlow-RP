#include "CutFlow4Lep/Cuts/AllPreselection.h"

AllPreselection::AllPreselection(D3PDReader::Event *tEvent, Int_t tDataYear) : Cuts(tEvent), m_dataYear(tDataYear)
{
}

AllPreselection::~AllPreselection() {}

Bool_t AllPreselection::passedCut()
{
	
}