#include "CutFlow4Lep/ChannelCutFlows/CutFlow4mu.h"

CutFlow4mu::CutFlow4mu(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<Muon*> *tInitMuonVec)
	: ChannelCutFlow(tEvent, tCurrMCCollection, tRunNumber_sf), m_initMuonVec(tInitMuonVec)
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

	Bool_t doZ4lAnalysis = true;
	QuadLepton* higgsEvent = getQuadEvent(&quadMuonVec, doZ4lAnalysis);

	// CutQuadLepton


}