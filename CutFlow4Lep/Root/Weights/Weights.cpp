#include "CutFlow4Lep\Weights\Weights.h"
#include "CutFlow4Lep\StructDef.h"

#include <ggFReweighting\ggFReweighting.h>

Weights::Weights(D3PDReader::Event *tEvent, Int_t tSampleType) : m_event(tEvent), m_sampleType(tSampleType)
{

}

Weights::~Weights()
{

}

Double_t Weights::getEventWeight()
{
	Double_t weight = 1;

	return weight;

}

Double_t Weights::getggFWeight()
{
	Double_t weight = 1;

	// Determining True Higgs transverse momentum
	Double_t trueHiggsPt = 0;
	for (Int_t i = 0; i < m_event->mc.n(); i++)
	{
		Int_t pdgID = TMath::Abs(m_event->mc[i].pdgId());
		Int_t status = m_event->mc[i].status();

		if (pdgID == 25 && (status == 2 || status == 10902 || status == 62))
			trueHiggsPt = m_event->mc[i].pt()/1000;
	}
	
	// Using ggF Reweighting to return proper weight
	pair<double, double> result;
	if (m_sampleType == SampleType::ggF || m_sampleType == SampleType::ggF_ZpZp)
	{
		result = ggFReweight->getWeightAndStatError(trueHiggsPt);
		weight = result.first;
	}

	return weight;
}

Double_t Weights::getHiggsWeight()
{

}

Double_t Weights::getJHUWeight()
{

}

Double_t Weights::getLepEffWeight()
{

}

Double_t Weights::getPileupWeight()
{

}

Double_t Weights::getTriggerWeight()
{

}

Double_t Weights::getZVertexWeight()
{

}