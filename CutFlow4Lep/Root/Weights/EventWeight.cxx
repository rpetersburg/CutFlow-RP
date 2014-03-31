#include "CutFlow4Lep/Weights/EventWeight.h"

EventWeight::EventWeight(D3PDReader::Event *tEvent, TString tDataYear, Double_t tHiggsMass,
												 Int_t tSampleType, Int_t tMCCollection, Int_t tDataCollection)
	:	Weights(tEvent, tDataYear)
{
	m_ggFWeight = (new ggFWeight(m_event, m_dataYear, tHiggsMass, tSampleType))->getWeight();
	m_jhuWeight = (new JHUWeight(m_event, m_dataYear))->getWeight();
	m_pileupWeight = (new PileupWeight(m_event, m_dataYear, tMCCollection, tDataCollection))->getWeight();
	m_zVertexWeight = (new ZVertexWeight(m_event, m_dataYear))->getWeight();

	setWeight();
}

EventWeight::~EventWeight() {}

void EventWeight::setWeight()
{
	m_weight *= m_event->eventinfo.mc_event_weight() * m_zVertexWeight * m_pileupWeight * m_ggFWeight * m_jhuWeight;
}