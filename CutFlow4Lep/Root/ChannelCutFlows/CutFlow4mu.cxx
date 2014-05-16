#include "CutFlow4Lep/ChannelCutFlows/CutFlow4mu.h"

CutFlow4mu::CutFlow4mu(vector<Muon*> *tInitMuonVec)
	: m_initMuonVec(tInitMuonVec)
{

}

CutFlow4mu::~CutFlow4mu()
{

}

Bool_t CutFlow4mu::passedCut()
{
	if (m_initMuonVec->size() < 4) return false;

	vector<DiLepton*> diMuonVec = getDiLeptons<Muon>(m_initMuonVec);
	if (diMuonVec.size() < 2) return false;

	vector<QuadLepton*> quadMuonVec = getQuadLeptons(&diMuonVec);
	if (quadMuonVec.size() < 1) return false;
}