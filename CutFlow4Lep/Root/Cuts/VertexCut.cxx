#include "CutFlow4Lep/Cuts/VertexCut.h"

VertexCut::VertexCut(D3PDReader::Event *tEvent) : Cuts(tEvent) {}

VertexCut::~VertexCut() {}

Bool_t VertexCut::passedCut()
{
	Int_t numVertex = m_event->vxp.n();
	for (Int_t i = 0; i < numVertex; i++)
		if (m_event->vxp[i].trk_n() >= 3) return true;
	return false;
}