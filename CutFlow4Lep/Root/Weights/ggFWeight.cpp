// This is only a test file if I eventually decide to implement inheritance with the weights

#include "CutFlow4Lep\Weights\ggFWeight.h"

ggFWeight::ggFWeight(D3PDReader::Event *tEvent, Int_t tSampleType, TString tDataYear) : Weights(tEvent, tSampleType, tDataYear)
{
	m_reweight = 0;
}

ggFWeight::~ggFWeight()
{
	delete m_event;
}

void ggFWeight::setWeight()
{
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
		m_weight = result.first;
	}
}

void ggFWeight::initializeReweight()
{
	
}
