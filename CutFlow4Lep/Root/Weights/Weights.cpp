#include "CutFlow4Lep\Weights\Weights.h"

Weights::Weights(D3PDReader::Event *tEvent, Int_t tSampleType, TString tDataYear) : m_event(tEvent), m_sampleType(tSampleType), m_dataYear(tDataYear)
{
	m_weight = 1;
}

Weights::~Weights()
{

}

Double_t Weights::getEventWeight()
{
	// Might need to modify weight depending on what type of weight is being tested

	// Get weight from event info
	Double_t weight = 1;
	Double_t weight = weight * m_event->eventinfo.mc_event_weight();
	return weight;
}

Double_t Weights::getggFWeight()
{
	Double_t weight = 1;

	// if (isMC);

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
	Double_t weight = 1 * getTriggerWeight(higgs) * getLepEffWeight(higgs);
	return weight;
}

Double_t Weights::getJHUWeight()
{
	Double_t weight = 1;

	// if (isMC);

	// Determining True Higgs transverse momentum
	Double_t trueHiggsPt = 0;
	for (Int_t i = 0; i < m_event->mc.n(); i++)
	{
		Int_t pdgID = TMath::Abs(m_event->mc[i].pdgId());
		Int_t status = m_event->mc[i].status();

		if ( (pdgID == 25 || pdgID == 39) && (status == 1 || status == 10902 || status == 62) )
			trueHiggsPt = m_event->mc[i].pt();
	}

	weight = ptJHUReweighting->GetJHUWeight(m_event->eventinfo.mc_channel_number(), trueHiggsPt/1000, 0);

	return weight;
}

Double_t Weights::getLepEffWeight()
{

}

Double_t Weights::getPileupWeight()
{
	// if (isMC);

	// Finding value mu
	Double_t mu = m_event->eventinfo.averageIntPerXing();
	if (m_dataYear == 2012) mu = (m_event->eventinfo.lbn() == 1 && int(mu+0.5) == 1) ? 0. : mu;

	// Setting weight based on mu
	Double_t weight = pileupTool->GetCombinedWeight(m_event->eventinfo.RunNumber(), m_event->eventinfo.mc_channel_number(), mu);
	return weight;
}

Double_t Weights::getTriggerWeight()
{

}

Double_t Weights::getZVertexWeight()
{
	// if (isMC);
	Double_t weight = 1;
	if (m_event->mc.n() > 1)
	{
		Double_t vxp_z = m_event->mc[2].vx_z();
		Double_t weight = zVertexTool->GetWeight(vxp_z);
	}
	return weight;
}