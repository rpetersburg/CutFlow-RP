#include "CutFlow4Lep/Weights/ZVertexWeight.h"

ZVertexWeight::ZVertexWeight(D3PDReader::Event *tEvent, TString tDataYear)
	:	Weights(tEvent, tDataYear)
{
	initializeReweight();
	setWeight();
}

ZVertexWeight::~ZVertexWeight() {}

void ZVertexWeight::initializeReweight()
{
	if (m_dataYear == 2011)
	{
		if (m_event->eventinfo.mc_channel_number() == 105200)
			m_reweight = new VertexPositionReweightingTool("s1272", "../../egammaAnalysisUtils/share/zvtx_weights_2011_2012.root");
		else
			m_reweight = new VertexPositionReweightingTool("s1310", "../../egammaAnalysisUtils/share/zvtx_weights_2011_2012.root");
	}
	else if (m_dataYear == 2012)
		m_reweight = new VertexPositionReweightingTool(VertexPositionReweightingTool::MC12a, "../../egammaAnalysisUtils/share/zvtx_weights_2011_2012.root");
	else cout << "Error: ZVertexWeight::initializeReweight(): Invalid data year" << endl;

}

void ZVertexWeight::setWeight()
{
	if (m_event->mc.n() > 1)
	{
		Double_t vxp_z = m_event->mc[2].vx_z();
		Double_t m_weight = m_reweight->GetWeight(vxp_z);
	}
}